/* Copyright (C) 2009-2023 Free Software Foundation, Inc.
   Contributed by Janne Blomqvist

This file is part of the GNU Fortran runtime library (libgfortran).

Libgfortran is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

Libgfortran is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#ifndef GFOR_FORMAT_H
#define GFOR_FORMAT_H

#include "io.h"

/* Format nodes.  A format string is converted into a tree of these
   structures, which is traversed as part of a data transfer statement.  */

struct fnode
{
  format_token format;
  int repeat;
  struct fnode *next;
  char *source;

  union
  {
    struct
    {
      int w, d, e;
    }
    real;

    struct
    {
      int length;
      char *p;
    }
    string;

    struct
    {
      int w, m;
    }
    integer;

    struct
    {
      char *string;
      int string_len;
      gfc_full_array_i4 *vlist;
    }
    udf;  /* User Defined Format.  */

    int w;
    int k;
    int r;
    int n;

    struct fnode *child;
  }
  u;

  /* Members for traversing the tree during data transfer.  */

  int count;
  struct fnode *current;

};


/* A storage structures for format node data.  */

#define FARRAY_SIZE 64

typedef struct fnode_array
{
  struct fnode_array *next;
  fnode array[FARRAY_SIZE];
}
fnode_array;


typedef struct format_data
{
  char *format_string, *string;
  const char *error;
  char error_element;
  format_token saved_token;
  int value, format_string_len, reversion_ok;
  fnode *avail;
  const fnode *saved_format;
  fnode_array *last;
  fnode_array array;
}
format_data;

extern void parse_format (st_parameter_dt *);
internal_proto(parse_format);

extern const fnode *next_format (st_parameter_dt *);
internal_proto(next_format);

extern void unget_format (st_parameter_dt *, const fnode *);
internal_proto(unget_format);

extern void format_error (st_parameter_dt *, const fnode *, const char *);
internal_proto(format_error);

extern void free_format_data (struct format_data *);
internal_proto(free_format_data);

extern void free_format (st_parameter_dt *);
internal_proto(free_format);

extern void free_format_hash_table (gfc_unit *);
internal_proto(free_format_hash_table);

extern void init_format_hash (st_parameter_dt *);
internal_proto(init_format_hash);

extern void free_format_hash (st_parameter_dt *);
internal_proto(free_format_hash);

#endif
