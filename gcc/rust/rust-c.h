/* rust-c.h -- Header file for rust frontend gcc C interface.
   Copyright (C) 2009-2019 Free Software Foundation, Inc.

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

#ifndef RUST_RUST_C_H
#define RUST_RUST_C_H

#define RUST_EXTERN_C

class Linemap;
class Backend;

/* Functions defined in the Rust frontend proper called by the GCC
   interface.  */

extern int rust_enable_dump (const char*);
extern int rust_enable_optimize (const char*, int);

extern void rust_add_search_path (const char*);

struct rust_create_rustgo_args
{
  int int_type_size;
  int pointer_size;
  const char* pkgpath;
  const char* prefix;
  const char* relative_import_path;
  Backend* backend;
  Linemap* linemap;
};

extern void rust_create_rustgo (const struct rust_create_rustgo_args*);

extern void rust_parse_input_files (const char**, unsigned int,
                                    bool only_check_syntax);
extern void rust_write_globals (void);

/* Functions defined in the GCC interface called by the Rust frontend
   proper.  */

extern void rust_preserve_from_gc (tree);

extern bool saw_errors (void);

extern const char *rust_localize_identifier (const char*);

extern unsigned int rust_field_alignment (tree);

extern void rust_imported_unsafe (void);

extern void rust_write_export_data (const char *, unsigned int);

extern const char *rust_read_export_data (int, off_t, char **, size_t *, int *);

extern GTY(()) tree rust_non_zero_struct;

#endif /* !defined(RUST_RUST_C_H) */
