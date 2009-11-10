/* go-c.h -- Header file for go frontend gcc C interface.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#ifndef GO_GO_C_H
#define GO_GO_C_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "machmode.h"

extern int go_enable_dump(const char*);

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
