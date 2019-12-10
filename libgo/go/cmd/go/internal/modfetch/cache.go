// Copyright 2018 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

package modfetch

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"path/filepath"
	"strings"

	"cmd/go/internal/base"
	"cmd/go/internal/cfg"
	"cmd/go/internal/lockedfile"
	"cmd/go/internal/modfetch/codehost"
	"cmd/go/internal/module"
	"cmd/go/internal/par"
	"cmd/go/internal/renameio"
	"cmd/go/internal/semver"
)

var QuietLookup bool // do not print about lookups

var PkgMod string // $GOPATH/pkg/mod; set by package modload

func cacheDir(path string) (string, error) {
	if PkgMod == "" {
		return "", fmt.Errorf("internal error: modfetch.PkgMod not set")
	}
	enc, err := module.EncodePath(path)
	if err != nil {
		return "", err
	}
	return filepath.Join(PkgMod, "cache/download", enc, "/@v"), nil
}

func CachePath(m module.Version, suffix string) (string, error) {
	dir, err := cacheDir(m.Path)
	if err != nil {
		return "", err
	}
	if !semver.IsValid(m.Version) {
		return "", fmt.Errorf("non-semver module version %q", m.Version)
	}
	if module.CanonicalVersion(m.Version) != m.Version {
		return "", fmt.Errorf("non-canonical module version %q", m.Version)
	}
	encVer, err := module.EncodeVersion(m.Version)
	if err != nil {
		return "", err
	}
	return filepath.Join(dir, encVer+"."+suffix), nil
}

// DownloadDir returns the directory to which m should be downloaded.
// Note that the directory may not yet exist.
func DownloadDir(m module.Version) (string, error) {
	if PkgMod == "" {
		return "", fmt.Errorf("internal error: modfetch.PkgMod not set")
	}
	enc, err := module.EncodePath(m.Path)
	if err != nil {
		return "", err
	}
	if !semver.IsValid(m.Version) {
		return "", fmt.Errorf("non-semver module version %q", m.Version)
	}
	if module.CanonicalVersion(m.Version) != m.Version {
		return "", fmt.Errorf("non-canonical module version %q", m.Version)
	}
	encVer, err := module.EncodeVersion(m.Version)
	if err != nil {
		return "", err
	}
	return filepath.Join(PkgMod, enc+"@"+encVer), nil
}

// lockVersion locks a file within the module cache that guards the downloading
// and extraction of the zipfile for the given module version.
func lockVersion(mod module.Version) (unlock func(), err error) {
	path, err := CachePath(mod, "lock")
	if err != nil {
		return nil, err
	}
	if err := os.MkdirAll(filepath.Dir(path), 0777); err != nil {
		return nil, err
	}
	return lockedfile.MutexAt(path).Lock()
}

// SideLock locks a file within the module cache that that guards edits to files
// outside the cache, such as go.sum and go.mod files in the user's working
// directory. It returns a function that must be called to unlock the file.
func SideLock() (unlock func()) {
	if PkgMod == "" {
		base.Fatalf("go: internal error: modfetch.PkgMod not set")
	}
	path := filepath.Join(PkgMod, "cache", "lock")
	if err := os.MkdirAll(filepath.Dir(path), 0777); err != nil {
		base.Fatalf("go: failed to create cache directory %s: %v", filepath.Dir(path), err)
	}
	unlock, err := lockedfile.MutexAt(path).Lock()
	if err != nil {
		base.Fatalf("go: failed to lock file at %v", path)
	}
	return unlock
}

// A cachingRepo is a cache around an underlying Repo,
// avoiding redundant calls to ModulePath, Versions, Stat, Latest, and GoMod (but not Zip).
// It is also safe for simultaneous use by multiple goroutines
// (so that it can be returned from Lookup multiple times).
// It serializes calls to the underlying Repo.
type cachingRepo struct {
	path  string
	cache par.Cache // cache for all operations
	r     Repo
}

func newCachingRepo(r Repo) *cachingRepo {
	return &cachingRepo{
		r:    r,
		path: r.ModulePath(),
	}
}

func (r *cachingRepo) ModulePath() string {
	return r.path
}

func (r *cachingRepo) Versions(prefix string) ([]string, error) {
	type cached struct {
		list []string
		err  error
	}
	c := r.cache.Do("versions:"+prefix, func() interface{} {
		list, err := r.r.Versions(prefix)
		return cached{list, err}
	}).(cached)

	if c.err != nil {
		return nil, c.err
	}
	return append([]string(nil), c.list...), nil
}

type cachedInfo struct {
	info *RevInfo
	err  error
}

func (r *cachingRepo) Stat(rev string) (*RevInfo, error) {
	c := r.cache.Do("stat:"+rev, func() interface{} {
		file, info, err := readDiskStat(r.path, rev)
		if err == nil {
			return cachedInfo{info, nil}
		}

		if !QuietLookup {
			fmt.Fprintf(os.Stderr, "go: finding %s %s\n", r.path, rev)
		}
		info, err = r.r.Stat(rev)
		if err == nil {
			// If we resolved, say, 1234abcde to v0.0.0-20180604122334-1234abcdef78,
			// then save the information under the proper version, for future use.
			if info.Version != rev {
				file, _ = CachePath(module.Version{Path: r.path, Version: info.Version}, "info")
				r.cache.Do("stat:"+info.Version, func() interface{} {
					return cachedInfo{info, err}
				})
			}

			if err := writeDiskStat(file, info); err != nil {
				fmt.Fprintf(os.Stderr, "go: writing stat cache: %v\n", err)
			}
		}
		return cachedInfo{info, err}
	}).(cachedInfo)

	if c.err != nil {
		return nil, c.err
	}
	info := *c.info
	return &info, nil
}

func (r *cachingRepo) Latest() (*RevInfo, error) {
	c := r.cache.Do("latest:", func() interface{} {
		if !QuietLookup {
			fmt.Fprintf(os.Stderr, "go: finding %s latest\n", r.path)
		}
		info, err := r.r.Latest()

		// Save info for likely future Stat call.
		if err == nil {
			r.cache.Do("stat:"+info.Version, func() interface{} {
				return cachedInfo{info, err}
			})
			if file, _, err := readDiskStat(r.path, info.Version); err != nil {
				writeDiskStat(file, info)
			}
		}

		return cachedInfo{info, err}
	}).(cachedInfo)

	if c.err != nil {
		return nil, c.err
	}
	info := *c.info
	return &info, nil
}

func (r *cachingRepo) GoMod(version string) ([]byte, error) {
	type cached struct {
		text []byte
		err  error
	}
	c := r.cache.Do("gomod:"+version, func() interface{} {
		file, text, err := readDiskGoMod(r.path, version)
		if err == nil {
			// Note: readDiskGoMod already called checkGoMod.
			return cached{text, nil}
		}

		text, err = r.r.GoMod(version)
		if err == nil {
			checkGoMod(r.path, version, text)
			if err := writeDiskGoMod(file, text); err != nil {
				fmt.Fprintf(os.Stderr, "go: writing go.mod cache: %v\n", err)
			}
		}
		return cached{text, err}
	}).(cached)

	if c.err != nil {
		return nil, c.err
	}
	return append([]byte(nil), c.text...), nil
}

func (r *cachingRepo) Zip(dst io.Writer, version string) error {
	return r.r.Zip(dst, version)
}

// Stat is like Lookup(path).Stat(rev) but avoids the
// repository path resolution in Lookup if the result is
// already cached on local disk.
func Stat(proxy, path, rev string) (*RevInfo, error) {
	_, info, err := readDiskStat(path, rev)
	if err == nil {
		return info, nil
	}
	repo, err := Lookup(proxy, path)
	if err != nil {
		return nil, err
	}
	return repo.Stat(rev)
}

// InfoFile is like Stat but returns the name of the file containing
// the cached information.
func InfoFile(path, version string) (string, error) {
	if !semver.IsValid(version) {
		return "", fmt.Errorf("invalid version %q", version)
	}

	if file, _, err := readDiskStat(path, version); err == nil {
		return file, nil
	}

	err := TryProxies(func(proxy string) error {
		repo, err := Lookup(proxy, path)
		if err == nil {
			_, err = repo.Stat(version)
		}
		return err
	})
	if err != nil {
		return "", err
	}

	// Stat should have populated the disk cache for us.
	file, _, err := readDiskStat(path, version)
	if err != nil {
		return "", err
	}
	return file, nil
}

// GoMod is like Lookup(path).GoMod(rev) but avoids the
// repository path resolution in Lookup if the result is
// already cached on local disk.
func GoMod(path, rev string) ([]byte, error) {
	// Convert commit hash to pseudo-version
	// to increase cache hit rate.
	if !semver.IsValid(rev) {
		if _, info, err := readDiskStat(path, rev); err == nil {
			rev = info.Version
		} else {
			err := TryProxies(func(proxy string) error {
				repo, err := Lookup(proxy, path)
				if err != nil {
					return err
				}
				info, err := repo.Stat(rev)
				if err == nil {
					rev = info.Version
				}
				return err
			})
			if err != nil {
				return nil, err
			}
		}
	}

	_, data, err := readDiskGoMod(path, rev)
	if err == nil {
		return data, nil
	}

	err = TryProxies(func(proxy string) error {
		repo, err := Lookup(proxy, path)
		if err == nil {
			data, err = repo.GoMod(rev)
		}
		return err
	})
	return data, err
}

// GoModFile is like GoMod but returns the name of the file containing
// the cached information.
func GoModFile(path, version string) (string, error) {
	if !semver.IsValid(version) {
		return "", fmt.Errorf("invalid version %q", version)
	}
	if _, err := GoMod(path, version); err != nil {
		return "", err
	}
	// GoMod should have populated the disk cache for us.
	file, _, err := readDiskGoMod(path, version)
	if err != nil {
		return "", err
	}
	return file, nil
}

// GoModSum returns the go.sum entry for the module version's go.mod file.
// (That is, it returns the entry listed in go.sum as "path version/go.mod".)
func GoModSum(path, version string) (string, error) {
	if !semver.IsValid(version) {
		return "", fmt.Errorf("invalid version %q", version)
	}
	data, err := GoMod(path, version)
	if err != nil {
		return "", err
	}
	sum, err := goModSum(data)
	if err != nil {
		return "", err
	}
	return sum, nil
}

var errNotCached = fmt.Errorf("not in cache")

// readDiskStat reads a cached stat result from disk,
// returning the name of the cache file and the result.
// If the read fails, the caller can use
// writeDiskStat(file, info) to write a new cache entry.
func readDiskStat(path, rev string) (file string, info *RevInfo, err error) {
	file, data, err := readDiskCache(path, rev, "info")
	if err != nil {
		// If the cache already contains a pseudo-version with the given hash, we
		// would previously return that pseudo-version without checking upstream.
		// However, that produced an unfortunate side-effect: if the author added a
		// tag to the repository, 'go get' would not pick up the effect of that new
		// tag on the existing commits, and 'go' commands that referred to those
		// commits would use the previous name instead of the new one.
		//
		// That's especially problematic if the original pseudo-version starts with
		// v0.0.0-, as was the case for all pseudo-versions during vgo development,
		// since a v0.0.0- pseudo-version has lower precedence than pretty much any
		// tagged version.
		//
		// In practice, we're only looking up by hash during initial conversion of a
		// legacy config and during an explicit 'go get', and a little extra latency
		// for those operations seems worth the benefit of picking up more accurate
		// versions.
		//
		// Fall back to this resolution scheme only if the GOPROXY setting prohibits
		// us from resolving upstream tags.
		if cfg.GOPROXY == "off" {
			if file, info, err := readDiskStatByHash(path, rev); err == nil {
				return file, info, nil
			}
		}
		return file, nil, err
	}
	info = new(RevInfo)
	if err := json.Unmarshal(data, info); err != nil {
		return file, nil, errNotCached
	}
	// The disk might have stale .info files that have Name and Short fields set.
	// We want to canonicalize to .info files with those fields omitted.
	// Remarshal and update the cache file if needed.
	data2, err := json.Marshal(info)
	if err == nil && !bytes.Equal(data2, data) {
		writeDiskCache(file, data)
	}
	return file, info, nil
}

// readDiskStatByHash is a fallback for readDiskStat for the case
// where rev is a commit hash instead of a proper semantic version.
// In that case, we look for a cached pseudo-version that matches
// the commit hash. If we find one, we use it.
// This matters most for converting legacy package management
// configs, when we are often looking up commits by full hash.
// Without this check we'd be doing network I/O to the remote repo
// just to find out about a commit we already know about
// (and have cached under its pseudo-version).
func readDiskStatByHash(path, rev string) (file string, info *RevInfo, err error) {
	if PkgMod == "" {
		// Do not download to current directory.
		return "", nil, errNotCached
	}

	if !codehost.AllHex(rev) || len(rev) < 12 {
		return "", nil, errNotCached
	}
	rev = rev[:12]
	cdir, err := cacheDir(path)
	if err != nil {
		return "", nil, errNotCached
	}
	dir, err := os.Open(cdir)
	if err != nil {
		return "", nil, errNotCached
	}
	names, err := dir.Readdirnames(-1)
	dir.Close()
	if err != nil {
		return "", nil, errNotCached
	}

	// A given commit hash may map to more than one pseudo-version,
	// depending on which tags are present on the repository.
	// Take the highest such version.
	var maxVersion string
	suffix := "-" + rev + ".info"
	err = errNotCached
	for _, name := range names {
		if strings.HasSuffix(name, suffix) {
			v := strings.TrimSuffix(name, ".info")
			if IsPseudoVersion(v) && semver.Max(maxVersion, v) == v {
				maxVersion = v
				file, info, err = readDiskStat(path, strings.TrimSuffix(name, ".info"))
			}
		}
	}
	return file, info, err
}

// oldVgoPrefix is the prefix in the old auto-generated cached go.mod files.
// We stopped trying to auto-generate the go.mod files. Now we use a trivial
// go.mod with only a module line, and we've dropped the version prefix
// entirely. If we see a version prefix, that means we're looking at an old copy
// and should ignore it.
var oldVgoPrefix = []byte("//vgo 0.0.")

// readDiskGoMod reads a cached go.mod file from disk,
// returning the name of the cache file and the result.
// If the read fails, the caller can use
// writeDiskGoMod(file, data) to write a new cache entry.
func readDiskGoMod(path, rev string) (file string, data []byte, err error) {
	file, data, err = readDiskCache(path, rev, "mod")

	// If the file has an old auto-conversion prefix, pretend it's not there.
	if bytes.HasPrefix(data, oldVgoPrefix) {
		err = errNotCached
		data = nil
	}

	if err == nil {
		checkGoMod(path, rev, data)
	}

	return file, data, err
}

// readDiskCache is the generic "read from a cache file" implementation.
// It takes the revision and an identifying suffix for the kind of data being cached.
// It returns the name of the cache file and the content of the file.
// If the read fails, the caller can use
// writeDiskCache(file, data) to write a new cache entry.
func readDiskCache(path, rev, suffix string) (file string, data []byte, err error) {
	file, err = CachePath(module.Version{Path: path, Version: rev}, suffix)
	if err != nil {
		return "", nil, errNotCached
	}
	data, err = renameio.ReadFile(file)
	if err != nil {
		return file, nil, errNotCached
	}
	return file, data, nil
}

// writeDiskStat writes a stat result cache entry.
// The file name must have been returned by a previous call to readDiskStat.
func writeDiskStat(file string, info *RevInfo) error {
	if file == "" {
		return nil
	}
	js, err := json.Marshal(info)
	if err != nil {
		return err
	}
	return writeDiskCache(file, js)
}

// writeDiskGoMod writes a go.mod cache entry.
// The file name must have been returned by a previous call to readDiskGoMod.
func writeDiskGoMod(file string, text []byte) error {
	return writeDiskCache(file, text)
}

// writeDiskCache is the generic "write to a cache file" implementation.
// The file must have been returned by a previous call to readDiskCache.
func writeDiskCache(file string, data []byte) error {
	if file == "" {
		return nil
	}
	// Make sure directory for file exists.
	if err := os.MkdirAll(filepath.Dir(file), 0777); err != nil {
		return err
	}

	if err := renameio.WriteFile(file, data, 0666); err != nil {
		return err
	}

	if strings.HasSuffix(file, ".mod") {
		rewriteVersionList(filepath.Dir(file))
	}
	return nil
}

// rewriteVersionList rewrites the version list in dir
// after a new *.mod file has been written.
func rewriteVersionList(dir string) {
	if filepath.Base(dir) != "@v" {
		base.Fatalf("go: internal error: misuse of rewriteVersionList")
	}

	listFile := filepath.Join(dir, "list")

	// We use a separate lockfile here instead of locking listFile itself because
	// we want to use Rename to write the file atomically. The list may be read by
	// a GOPROXY HTTP server, and if we crash midway through a rewrite (or if the
	// HTTP server ignores our locking and serves the file midway through a
	// rewrite) it's better to serve a stale list than a truncated one.
	unlock, err := lockedfile.MutexAt(listFile + ".lock").Lock()
	if err != nil {
		base.Fatalf("go: can't lock version list lockfile: %v", err)
	}
	defer unlock()

	infos, err := ioutil.ReadDir(dir)
	if err != nil {
		return
	}
	var list []string
	for _, info := range infos {
		// We look for *.mod files on the theory that if we can't supply
		// the .mod file then there's no point in listing that version,
		// since it's unusable. (We can have *.info without *.mod.)
		// We don't require *.zip files on the theory that for code only
		// involved in module graph construction, many *.zip files
		// will never be requested.
		name := info.Name()
		if strings.HasSuffix(name, ".mod") {
			v := strings.TrimSuffix(name, ".mod")
			if v != "" && module.CanonicalVersion(v) == v {
				list = append(list, v)
			}
		}
	}
	SortVersions(list)

	var buf bytes.Buffer
	for _, v := range list {
		buf.WriteString(v)
		buf.WriteString("\n")
	}
	old, _ := renameio.ReadFile(listFile)
	if bytes.Equal(buf.Bytes(), old) {
		return
	}

	if err := renameio.WriteFile(listFile, buf.Bytes(), 0666); err != nil {
		base.Fatalf("go: failed to write version list: %v", err)
	}
}
