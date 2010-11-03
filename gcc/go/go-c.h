/* go-c.h -- Header file for go frontend gcc C interface.
   Copyright (C) 2009, 2010 Free Software Foundation, Inc.

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

#ifndef GO_GO_C_H
#define GO_GO_C_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "machmode.h"

extern int go_enable_dump(const char*);
extern void go_set_prefix(const char*);

extern void go_preserve_from_gc(tree);

extern void go_add_search_path(const char*);
extern void go_create_gogo(void);
extern void go_parse_input_files(const char**, unsigned int);
extern void go_write_globals(void);

extern tree go_type_for_size(unsigned int bits, int unsignedp);
extern tree go_type_for_mode(enum machine_mode, int unsignedp);

#ifdef __cplusplus
} /* End extern "C".  */
#endif

#endif /* !defined(GO_GO_C_H) */
