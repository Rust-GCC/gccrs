// Copyright 2018 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

package modcmd

import (
	"cmd/go/internal/cfg"
	"encoding/json"
	"os"

	"cmd/go/internal/base"
	"cmd/go/internal/modfetch"
	"cmd/go/internal/modload"
	"cmd/go/internal/module"
	"cmd/go/internal/par"
)

var cmdDownload = &base.Command{
	UsageLine: "go mod download [-json] [modules]",
	Short:     "download modules to local cache",
	Long: `
Download downloads the named modules, which can be module patterns selecting
dependencies of the main module or module queries of the form path@version.
With no arguments, download applies to all dependencies of the main module.

The go command will automatically download modules as needed during ordinary
execution. The "go mod download" command is useful mainly for pre-filling
the local cache or to compute the answers for a Go module proxy.

By default, download reports errors to standard error but is otherwise silent.
The -json flag causes download to print a sequence of JSON objects
to standard output, describing each downloaded module (or failure),
corresponding to this Go struct:

    type Module struct {
        Path     string // module path
        Version  string // module version
        Error    string // error loading module
        Info     string // absolute path to cached .info file
        GoMod    string // absolute path to cached .mod file
        Zip      string // absolute path to cached .zip file
        Dir      string // absolute path to cached source root directory
        Sum      string // checksum for path, version (as in go.sum)
        GoModSum string // checksum for go.mod (as in go.sum)
        Latest   bool   // would @latest resolve to this version?
    }

See 'go help modules' for more about module queries.
	`,
}

var downloadJSON = cmdDownload.Flag.Bool("json", false, "")

func init() {
	cmdDownload.Run = runDownload // break init cycle
}

type moduleJSON struct {
	Path     string `json:",omitempty"`
	Version  string `json:",omitempty"`
	Error    string `json:",omitempty"`
	Info     string `json:",omitempty"`
	GoMod    string `json:",omitempty"`
	Zip      string `json:",omitempty"`
	Dir      string `json:",omitempty"`
	Sum      string `json:",omitempty"`
	GoModSum string `json:",omitempty"`
	Latest   bool   `json:",omitempty"`
}

func runDownload(cmd *base.Command, args []string) {
	// Check whether modules are enabled and whether we're in a module.
	if cfg.Getenv("GO111MODULE") == "off" {
		base.Fatalf("go: modules disabled by GO111MODULE=off; see 'go help modules'")
	}
	if !modload.HasModRoot() && len(args) == 0 {
		base.Fatalf("go mod download: no modules specified (see 'go help mod download')")
	}
	if len(args) == 0 {
		args = []string{"all"}
	}

	var mods []*moduleJSON
	var work par.Work
	listU := false
	listVersions := false
	for _, info := range modload.ListModules(args, listU, listVersions) {
		if info.Replace != nil {
			info = info.Replace
		}
		if info.Version == "" && info.Error == nil {
			// main module
			continue
		}
		m := &moduleJSON{
			Path:    info.Path,
			Version: info.Version,
		}
		mods = append(mods, m)
		if info.Error != nil {
			m.Error = info.Error.Err
			continue
		}
		work.Add(m)
	}

	latest := map[string]string{} // path → version
	if *downloadJSON {
		// We need to populate the Latest field, but if the main module depends on a
		// version newer than latest — or if the version requested on the command
		// line is itself newer than latest — that's not trivial to determine from
		// the info returned by ListModules. Instead, we issue a separate
		// ListModules request for "latest", which should be inexpensive relative to
		// downloading the modules.
		var latestArgs []string
		for _, m := range mods {
			if m.Error != "" {
				continue
			}
			latestArgs = append(latestArgs, m.Path+"@latest")
		}

		if len(latestArgs) > 0 {
			for _, info := range modload.ListModules(latestArgs, listU, listVersions) {
				if info.Version != "" {
					latest[info.Path] = info.Version
				}
			}
		}
	}

	work.Do(10, func(item interface{}) {
		m := item.(*moduleJSON)
		var err error
		m.Info, err = modfetch.InfoFile(m.Path, m.Version)
		if err != nil {
			m.Error = err.Error()
			return
		}
		m.GoMod, err = modfetch.GoModFile(m.Path, m.Version)
		if err != nil {
			m.Error = err.Error()
			return
		}
		m.GoModSum, err = modfetch.GoModSum(m.Path, m.Version)
		if err != nil {
			m.Error = err.Error()
			return
		}
		mod := module.Version{Path: m.Path, Version: m.Version}
		m.Zip, err = modfetch.DownloadZip(mod)
		if err != nil {
			m.Error = err.Error()
			return
		}
		m.Sum = modfetch.Sum(mod)
		m.Dir, err = modfetch.Download(mod)
		if err != nil {
			m.Error = err.Error()
			return
		}
		if latest[m.Path] == m.Version {
			m.Latest = true
		}
	})

	if *downloadJSON {
		for _, m := range mods {
			b, err := json.MarshalIndent(m, "", "\t")
			if err != nil {
				base.Fatalf("%v", err)
			}
			os.Stdout.Write(append(b, '\n'))
			if m.Error != "" {
				base.SetExitStatus(1)
			}
		}
	} else {
		for _, m := range mods {
			if m.Error != "" {
				base.Errorf("%s", m.Error)
			}
		}
		base.ExitIfErrors()
	}
}
