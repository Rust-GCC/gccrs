/* rust-gcc.h -- Header file for rust backend-specific interfaces.
   Copyright (C) 2019 Free Software Foundation, Inc.

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

#ifndef RUST_RUST_GCC_BACKEND_H
#define RUST_RUST_GCC_BACKEND_H

class Backend;

// Create and return a Backend object for use with the GCC backend.

extern Backend *rust_get_backend();

// Create and return a Linemap object for use with the GCC backend.

extern Linemap *rust_get_linemap();

#endif // !defined(RUST_GCC_BACKEND_H)
