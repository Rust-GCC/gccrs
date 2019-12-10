// Copyright 2018 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

package modload

import (
	"bytes"
	"errors"
	"fmt"
	"go/build"
	"internal/goroot"
	"os"
	"path/filepath"
	"sort"
	"strings"
	"time"

	"cmd/go/internal/cfg"
	"cmd/go/internal/modfetch"
	"cmd/go/internal/module"
	"cmd/go/internal/par"
	"cmd/go/internal/search"
	"cmd/go/internal/semver"
	"cmd/go/internal/str"
)

type ImportMissingError struct {
	ImportPath string
	Module     module.Version

	// newMissingVersion is set to a newer version of Module if one is present
	// in the build list. When set, we can't automatically upgrade.
	newMissingVersion string
}

func (e *ImportMissingError) Error() string {
	if e.Module.Path == "" {
		if str.HasPathPrefix(e.ImportPath, "cmd") {
			return fmt.Sprintf("package %s is not in GOROOT (%s)", e.ImportPath, filepath.Join(cfg.GOROOT, "src", e.ImportPath))
		}
		return "cannot find module providing package " + e.ImportPath
	}
	return "missing module for import: " + e.Module.Path + "@" + e.Module.Version + " provides " + e.ImportPath
}

// Import finds the module and directory in the build list
// containing the package with the given import path.
// The answer must be unique: Import returns an error
// if multiple modules attempt to provide the same package.
// Import can return a module with an empty m.Path, for packages in the standard library.
// Import can return an empty directory string, for fake packages like "C" and "unsafe".
//
// If the package cannot be found in the current build list,
// Import returns an ImportMissingError as the error.
// If Import can identify a module that could be added to supply the package,
// the ImportMissingError records that module.
func Import(path string) (m module.Version, dir string, err error) {
	if strings.Contains(path, "@") {
		return module.Version{}, "", fmt.Errorf("import path should not have @version")
	}
	if build.IsLocalImport(path) {
		return module.Version{}, "", fmt.Errorf("relative import not supported")
	}
	if path == "C" || path == "unsafe" {
		// There's no directory for import "C" or import "unsafe".
		return module.Version{}, "", nil
	}

	// Is the package in the standard library?
	if search.IsStandardImportPath(path) &&
		goroot.IsStandardPackage(cfg.GOROOT, cfg.BuildContext.Compiler, path) {
		if targetInGorootSrc {
			if dir, ok := dirInModule(path, targetPrefix, ModRoot(), true); ok {
				return Target, dir, nil
			}
		}
		dir := filepath.Join(cfg.GOROOT, "src", path)
		return module.Version{}, dir, nil
	}
	if str.HasPathPrefix(path, "cmd") {
		return module.Version{}, "", &ImportMissingError{ImportPath: path}
	}

	// -mod=vendor is special.
	// Everything must be in the main module or the main module's vendor directory.
	if cfg.BuildMod == "vendor" {
		mainDir, mainOK := dirInModule(path, targetPrefix, ModRoot(), true)
		vendorDir, vendorOK := dirInModule(path, "", filepath.Join(ModRoot(), "vendor"), false)
		if mainOK && vendorOK {
			return module.Version{}, "", fmt.Errorf("ambiguous import: found %s in multiple directories:\n\t%s\n\t%s", path, mainDir, vendorDir)
		}
		// Prefer to return main directory if there is one,
		// Note that we're not checking that the package exists.
		// We'll leave that for load.
		if !vendorOK && mainDir != "" {
			return Target, mainDir, nil
		}
		readVendorList()
		return vendorMap[path], vendorDir, nil
	}

	// Check each module on the build list.
	var dirs []string
	var mods []module.Version
	for _, m := range buildList {
		if !maybeInModule(path, m.Path) {
			// Avoid possibly downloading irrelevant modules.
			continue
		}
		root, isLocal, err := fetch(m)
		if err != nil {
			// Report fetch error.
			// Note that we don't know for sure this module is necessary,
			// but it certainly _could_ provide the package, and even if we
			// continue the loop and find the package in some other module,
			// we need to look at this module to make sure the import is
			// not ambiguous.
			return module.Version{}, "", err
		}
		dir, ok := dirInModule(path, m.Path, root, isLocal)
		if ok {
			mods = append(mods, m)
			dirs = append(dirs, dir)
		}
	}
	if len(mods) == 1 {
		return mods[0], dirs[0], nil
	}
	if len(mods) > 0 {
		var buf bytes.Buffer
		fmt.Fprintf(&buf, "ambiguous import: found %s in multiple modules:", path)
		for i, m := range mods {
			fmt.Fprintf(&buf, "\n\t%s", m.Path)
			if m.Version != "" {
				fmt.Fprintf(&buf, " %s", m.Version)
			}
			fmt.Fprintf(&buf, " (%s)", dirs[i])
		}
		return module.Version{}, "", errors.New(buf.String())
	}

	// Look up module containing the package, for addition to the build list.
	// Goal is to determine the module, download it to dir, and return m, dir, ErrMissing.
	if cfg.BuildMod == "readonly" {
		return module.Version{}, "", fmt.Errorf("import lookup disabled by -mod=%s", cfg.BuildMod)
	}

	// Not on build list.
	// To avoid spurious remote fetches, next try the latest replacement for each module.
	// (golang.org/issue/26241)
	if modFile != nil {
		latest := map[string]string{} // path -> version
		for _, r := range modFile.Replace {
			if maybeInModule(path, r.Old.Path) {
				latest[r.Old.Path] = semver.Max(r.Old.Version, latest[r.Old.Path])
			}
		}

		mods = make([]module.Version, 0, len(latest))
		for p, v := range latest {
			// If the replacement didn't specify a version, synthesize a
			// pseudo-version with an appropriate major version and a timestamp below
			// any real timestamp. That way, if the main module is used from within
			// some other module, the user will be able to upgrade the requirement to
			// any real version they choose.
			if v == "" {
				if _, pathMajor, ok := module.SplitPathVersion(p); ok && len(pathMajor) > 0 {
					v = modfetch.PseudoVersion(pathMajor[1:], "", time.Time{}, "000000000000")
				} else {
					v = modfetch.PseudoVersion("v0", "", time.Time{}, "000000000000")
				}
			}
			mods = append(mods, module.Version{Path: p, Version: v})
		}

		// Every module path in mods is a prefix of the import path.
		// As in QueryPackage, prefer the longest prefix that satisfies the import.
		sort.Slice(mods, func(i, j int) bool {
			return len(mods[i].Path) > len(mods[j].Path)
		})
		for _, m := range mods {
			root, isLocal, err := fetch(m)
			if err != nil {
				// Report fetch error as above.
				return module.Version{}, "", err
			}
			_, ok := dirInModule(path, m.Path, root, isLocal)
			if ok {
				return m, "", &ImportMissingError{ImportPath: path, Module: m}
			}
		}
	}

	candidates, err := QueryPackage(path, "latest", Allowed)
	if err != nil {
		if errors.Is(err, os.ErrNotExist) {
			// Return "cannot find module providing package […]" instead of whatever
			// low-level error QueryPackage produced.
			return module.Version{}, "", &ImportMissingError{ImportPath: path}
		} else {
			return module.Version{}, "", err
		}
	}
	m = candidates[0].Mod
	newMissingVersion := ""
	for _, c := range candidates {
		cm := c.Mod
		for _, bm := range buildList {
			if bm.Path == cm.Path && semver.Compare(bm.Version, cm.Version) > 0 {
				// QueryPackage proposed that we add module cm to provide the package,
				// but we already depend on a newer version of that module (and we don't
				// have the package).
				//
				// This typically happens when a package is present at the "@latest"
				// version (e.g., v1.0.0) of a module, but we have a newer version
				// of the same module in the build list (e.g., v1.0.1-beta), and
				// the package is not present there.
				m = cm
				newMissingVersion = bm.Version
				break
			}
		}
	}
	return m, "", &ImportMissingError{ImportPath: path, Module: m, newMissingVersion: newMissingVersion}
}

// maybeInModule reports whether, syntactically,
// a package with the given import path could be supplied
// by a module with the given module path (mpath).
func maybeInModule(path, mpath string) bool {
	return mpath == path ||
		len(path) > len(mpath) && path[len(mpath)] == '/' && path[:len(mpath)] == mpath
}

var haveGoModCache, haveGoFilesCache par.Cache

// dirInModule locates the directory that would hold the package named by the given path,
// if it were in the module with module path mpath and root mdir.
// If path is syntactically not within mpath,
// or if mdir is a local file tree (isLocal == true) and the directory
// that would hold path is in a sub-module (covered by a go.mod below mdir),
// dirInModule returns "", false.
//
// Otherwise, dirInModule returns the name of the directory where
// Go source files would be expected, along with a boolean indicating
// whether there are in fact Go source files in that directory.
func dirInModule(path, mpath, mdir string, isLocal bool) (dir string, haveGoFiles bool) {
	// Determine where to expect the package.
	if path == mpath {
		dir = mdir
	} else if mpath == "" { // vendor directory
		dir = filepath.Join(mdir, path)
	} else if len(path) > len(mpath) && path[len(mpath)] == '/' && path[:len(mpath)] == mpath {
		dir = filepath.Join(mdir, path[len(mpath)+1:])
	} else {
		return "", false
	}

	// Check that there aren't other modules in the way.
	// This check is unnecessary inside the module cache
	// and important to skip in the vendor directory,
	// where all the module trees have been overlaid.
	// So we only check local module trees
	// (the main module, and any directory trees pointed at by replace directives).
	if isLocal {
		for d := dir; d != mdir && len(d) > len(mdir); {
			haveGoMod := haveGoModCache.Do(d, func() interface{} {
				fi, err := os.Stat(filepath.Join(d, "go.mod"))
				return err == nil && !fi.IsDir()
			}).(bool)

			if haveGoMod {
				return "", false
			}
			parent := filepath.Dir(d)
			if parent == d {
				// Break the loop, as otherwise we'd loop
				// forever if d=="." and mdir=="".
				break
			}
			d = parent
		}
	}

	// Now committed to returning dir (not "").

	// Are there Go source files in the directory?
	// We don't care about build tags, not even "+build ignore".
	// We're just looking for a plausible directory.
	haveGoFiles = haveGoFilesCache.Do(dir, func() interface{} {
		f, err := os.Open(dir)
		if err != nil {
			return false
		}
		defer f.Close()
		names, _ := f.Readdirnames(-1)
		for _, name := range names {
			if strings.HasSuffix(name, ".go") {
				info, err := os.Stat(filepath.Join(dir, name))
				if err == nil && info.Mode().IsRegular() {
					return true
				}
			}
		}
		return false
	}).(bool)

	return dir, haveGoFiles
}
