/* lang-specs.h -- gcc driver specs for Go frontend.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

/* This is the contribution to the `default_compilers' array in gcc.c
   for the Go language.  */

{".go",  "@go", 0, 1, 0},
{"@go",  "go1 %i %(cc1_options) %{I*} %{L*} %D %{!fsyntax-only:%(invoke_as)}",
    0, 1, 0},
