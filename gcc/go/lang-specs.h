/* lang-specs.h -- gcc driver specs for Go frontend.
   Copyright (C) 2009-2023 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

/* This is the contribution to the `default_compilers' array in gcc.cc
   for the Go language.  */

{".go",  "@go", 0, 1, 0},
{"@go",  "go1 %i %(cc1_options) %{I*} %{L*} %D %{!fsyntax-only:%(invoke_as)}",
    0, 1, 0},
