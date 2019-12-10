// Copyright 2012 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// Package clean implements the ``go clean'' command.
package clean

import (
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"strconv"
	"strings"
	"time"

	"cmd/go/internal/base"
	"cmd/go/internal/cache"
	"cmd/go/internal/cfg"
	"cmd/go/internal/load"
	"cmd/go/internal/lockedfile"
	"cmd/go/internal/modfetch"
	"cmd/go/internal/modload"
	"cmd/go/internal/work"
)

var CmdClean = &base.Command{
	UsageLine: "go clean [clean flags] [build flags] [packages]",
	Short:     "remove object files and cached files",
	Long: `
Clean removes object files from package source directories.
The go command builds most objects in a temporary directory,
so go clean is mainly concerned with object files left by other
tools or by manual invocations of go build.

If a package argument is given or the -i or -r flag is set,
clean removes the following files from each of the
source directories corresponding to the import paths:

	_obj/            old object directory, left from Makefiles
	_test/           old test directory, left from Makefiles
	_testmain.go     old gotest file, left from Makefiles
	test.out         old test log, left from Makefiles
	build.out        old test log, left from Makefiles
	*.[568ao]        object files, left from Makefiles

	DIR(.exe)        from go build
	DIR.test(.exe)   from go test -c
	MAINFILE(.exe)   from go build MAINFILE.go
	*.so             from SWIG

In the list, DIR represents the final path element of the
directory, and MAINFILE is the base name of any Go source
file in the directory that is not included when building
the package.

The -i flag causes clean to remove the corresponding installed
archive or binary (what 'go install' would create).

The -n flag causes clean to print the remove commands it would execute,
but not run them.

The -r flag causes clean to be applied recursively to all the
dependencies of the packages named by the import paths.

The -x flag causes clean to print remove commands as it executes them.

The -cache flag causes clean to remove the entire go build cache.

The -testcache flag causes clean to expire all test results in the
go build cache.

The -modcache flag causes clean to remove the entire module
download cache, including unpacked source code of versioned
dependencies.

For more about build flags, see 'go help build'.

For more about specifying packages, see 'go help packages'.
	`,
}

var (
	cleanI         bool // clean -i flag
	cleanR         bool // clean -r flag
	cleanCache     bool // clean -cache flag
	cleanModcache  bool // clean -modcache flag
	cleanTestcache bool // clean -testcache flag
)

func init() {
	// break init cycle
	CmdClean.Run = runClean

	CmdClean.Flag.BoolVar(&cleanI, "i", false, "")
	CmdClean.Flag.BoolVar(&cleanR, "r", false, "")
	CmdClean.Flag.BoolVar(&cleanCache, "cache", false, "")
	CmdClean.Flag.BoolVar(&cleanModcache, "modcache", false, "")
	CmdClean.Flag.BoolVar(&cleanTestcache, "testcache", false, "")

	// -n and -x are important enough to be
	// mentioned explicitly in the docs but they
	// are part of the build flags.

	work.AddBuildFlags(CmdClean)
}

func runClean(cmd *base.Command, args []string) {
	// golang.org/issue/29925: only load packages before cleaning if
	// either the flags and arguments explicitly imply a package,
	// or no other target (such as a cache) was requested to be cleaned.
	cleanPkg := len(args) > 0 || cleanI || cleanR
	if (!modload.Enabled() || modload.HasModRoot()) &&
		!cleanCache && !cleanModcache && !cleanTestcache {
		cleanPkg = true
	}

	if cleanPkg {
		for _, pkg := range load.PackagesAndErrors(args) {
			clean(pkg)
		}
	}

	var b work.Builder
	b.Print = fmt.Print

	if cleanCache {
		dir := cache.DefaultDir()
		if dir != "off" {
			// Remove the cache subdirectories but not the top cache directory.
			// The top cache directory may have been created with special permissions
			// and not something that we want to remove. Also, we'd like to preserve
			// the access log for future analysis, even if the cache is cleared.
			subdirs, _ := filepath.Glob(filepath.Join(dir, "[0-9a-f][0-9a-f]"))
			printedErrors := false
			if len(subdirs) > 0 {
				if cfg.BuildN || cfg.BuildX {
					b.Showcmd("", "rm -r %s", strings.Join(subdirs, " "))
				}
				for _, d := range subdirs {
					// Only print the first error - there may be many.
					// This also mimics what os.RemoveAll(dir) would do.
					if err := os.RemoveAll(d); err != nil && !printedErrors {
						printedErrors = true
						base.Errorf("go clean -cache: %v", err)
					}
				}
			}

			logFile := filepath.Join(dir, "log.txt")
			if err := os.RemoveAll(logFile); err != nil && !printedErrors {
				printedErrors = true
				base.Errorf("go clean -cache: %v", err)
			}
		}
	}

	if cleanTestcache && !cleanCache {
		// Instead of walking through the entire cache looking for test results,
		// we write a file to the cache indicating that all test results from before
		// right now are to be ignored.
		dir := cache.DefaultDir()
		if dir != "off" {
			f, err := lockedfile.Edit(filepath.Join(dir, "testexpire.txt"))
			if err == nil {
				now := time.Now().UnixNano()
				buf, _ := ioutil.ReadAll(f)
				prev, _ := strconv.ParseInt(strings.TrimSpace(string(buf)), 10, 64)
				if now > prev {
					if err = f.Truncate(0); err == nil {
						if _, err = f.Seek(0, 0); err == nil {
							_, err = fmt.Fprintf(f, "%d\n", now)
						}
					}
				}
				if closeErr := f.Close(); err == nil {
					err = closeErr
				}
			}
			if err != nil {
				base.Errorf("go clean -testcache: %v", err)
			}
		}
	}

	if cleanModcache {
		if modfetch.PkgMod == "" {
			base.Fatalf("go clean -modcache: no module cache")
		}
		if cfg.BuildN || cfg.BuildX {
			b.Showcmd("", "rm -rf %s", modfetch.PkgMod)
		}
		if !cfg.BuildN {
			if err := modfetch.RemoveAll(modfetch.PkgMod); err != nil {
				base.Errorf("go clean -modcache: %v", err)
			}
		}
	}
}

var cleaned = map[*load.Package]bool{}

// TODO: These are dregs left by Makefile-based builds.
// Eventually, can stop deleting these.
var cleanDir = map[string]bool{
	"_test": true,
	"_obj":  true,
}

var cleanFile = map[string]bool{
	"_testmain.go": true,
	"test.out":     true,
	"build.out":    true,
	"a.out":        true,
}

var cleanExt = map[string]bool{
	".5":  true,
	".6":  true,
	".8":  true,
	".a":  true,
	".o":  true,
	".so": true,
}

func clean(p *load.Package) {
	if cleaned[p] {
		return
	}
	cleaned[p] = true

	if p.Dir == "" {
		base.Errorf("can't load package: %v", p.Error)
		return
	}
	dirs, err := ioutil.ReadDir(p.Dir)
	if err != nil {
		base.Errorf("go clean %s: %v", p.Dir, err)
		return
	}

	var b work.Builder
	b.Print = fmt.Print

	packageFile := map[string]bool{}
	if p.Name != "main" {
		// Record which files are not in package main.
		// The others are.
		keep := func(list []string) {
			for _, f := range list {
				packageFile[f] = true
			}
		}
		keep(p.GoFiles)
		keep(p.CgoFiles)
		keep(p.TestGoFiles)
		keep(p.XTestGoFiles)
	}

	_, elem := filepath.Split(p.Dir)
	var allRemove []string

	// Remove dir-named executable only if this is package main.
	if p.Name == "main" {
		allRemove = append(allRemove,
			elem,
			elem+".exe",
		)
	}

	// Remove package test executables.
	allRemove = append(allRemove,
		elem+".test",
		elem+".test.exe",
	)

	// Remove a potential executable for each .go file in the directory that
	// is not part of the directory's package.
	for _, dir := range dirs {
		name := dir.Name()
		if packageFile[name] {
			continue
		}
		if !dir.IsDir() && strings.HasSuffix(name, ".go") {
			// TODO(adg,rsc): check that this .go file is actually
			// in "package main", and therefore capable of building
			// to an executable file.
			base := name[:len(name)-len(".go")]
			allRemove = append(allRemove, base, base+".exe")
		}
	}

	if cfg.BuildN || cfg.BuildX {
		b.Showcmd(p.Dir, "rm -f %s", strings.Join(allRemove, " "))
	}

	toRemove := map[string]bool{}
	for _, name := range allRemove {
		toRemove[name] = true
	}
	for _, dir := range dirs {
		name := dir.Name()
		if dir.IsDir() {
			// TODO: Remove once Makefiles are forgotten.
			if cleanDir[name] {
				if cfg.BuildN || cfg.BuildX {
					b.Showcmd(p.Dir, "rm -r %s", name)
					if cfg.BuildN {
						continue
					}
				}
				if err := os.RemoveAll(filepath.Join(p.Dir, name)); err != nil {
					base.Errorf("go clean: %v", err)
				}
			}
			continue
		}

		if cfg.BuildN {
			continue
		}

		if cleanFile[name] || cleanExt[filepath.Ext(name)] || toRemove[name] {
			removeFile(filepath.Join(p.Dir, name))
		}
	}

	if cleanI && p.Target != "" {
		if cfg.BuildN || cfg.BuildX {
			b.Showcmd("", "rm -f %s", p.Target)
		}
		if !cfg.BuildN {
			removeFile(p.Target)
		}
	}

	if cleanR {
		for _, p1 := range p.Internal.Imports {
			clean(p1)
		}
	}
}

// removeFile tries to remove file f, if error other than file doesn't exist
// occurs, it will report the error.
func removeFile(f string) {
	err := os.Remove(f)
	if err == nil || os.IsNotExist(err) {
		return
	}
	// Windows does not allow deletion of a binary file while it is executing.
	if base.ToolIsWindows {
		// Remove lingering ~ file from last attempt.
		if _, err2 := os.Stat(f + "~"); err2 == nil {
			os.Remove(f + "~")
		}
		// Try to move it out of the way. If the move fails,
		// which is likely, we'll try again the
		// next time we do an install of this binary.
		if err2 := os.Rename(f, f+"~"); err2 == nil {
			os.Remove(f + "~")
			return
		}
	}
	base.Errorf("go clean: %v", err)
}
