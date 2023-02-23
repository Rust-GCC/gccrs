/* rust-backend.c -- Rust frontend interface to gcc backend.
   Copyright (C) 2010-2023 Free Software Foundation, Inc.

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

#include "rust-system.h"
#include "coretypes.h"
#include "target.h"
#include "tree.h"
#include "memmodel.h"
#include "tm_p.h"
#include "diagnostic.h"
#include "simple-object.h"
#include "stor-layout.h"
#include "intl.h"
#include "output.h" /* for assemble_string */
#include "common/common-target.h"

// satisfy intellisense
#include "options.h"

/* The segment name we pass to simple_object_start_read to find Rust
   export data.  */

#ifndef RUST_EXPORT_SEGMENT_NAME
#define RUST_EXPORT_SEGMENT_NAME "__GNU_RUST"
#endif

/* The section name we use when reading and writing export data.  */

#ifndef RUST_EXPORT_SECTION_NAME
#define RUST_EXPORT_SECTION_NAME ".rust_export"
#endif

#ifndef TARGET_AIX
#define TARGET_AIX 0
#endif

/* Return whether or not GCC has reported any errors.  */

bool
saw_errors (void)
{
  return errorcount != 0 || sorrycount != 0;
}

/* Return the alignment in bytes of a struct field of type T.  */

unsigned int
rust_field_alignment (tree t)
{
  unsigned int v;

  v = TYPE_ALIGN (t);

#ifdef BIGGEST_FIELD_ALIGNMENT
  if (v > BIGGEST_FIELD_ALIGNMENT)
    v = BIGGEST_FIELD_ALIGNMENT;
#endif

#ifdef ADJUST_FIELD_ALIGN
  v = ADJUST_FIELD_ALIGN (NULL_TREE, t, v);
#endif

  return v / BITS_PER_UNIT;
}

/* This is called by the Rust frontend proper to add data to the
   section containing Rust export data.  */

void
rust_write_export_data (const char *bytes, unsigned int size)
{
  static section *sec;

  if (sec == NULL)
    {
      gcc_assert (targetm_common.have_named_sections);
      sec = get_section (RUST_EXPORT_SECTION_NAME,
			 TARGET_AIX ? SECTION_EXCLUDE : SECTION_DEBUG, NULL);
    }

  switch_to_section (sec);
  assemble_string (bytes, size);
}

/* The rust_read_export_data function is called by the Rust frontend
   proper to read Rust export data from an object file.  FD is a file
   descriptor open for reading.  OFFSET is the offset within the file
   where the object file starts; this will be 0 except when reading an
   archive.  On success this returns NULL and sets *PBUF to a buffer
   allocated using malloc, of size *PLEN, holding the export data.  If
   the data is not found, this returns NULL and sets *PBUF to NULL and
   *PLEN to 0.  If some error occurs, this returns an error message
   and sets *PERR to an errno value or 0 if there is no relevant
   errno.  */

const char *
rust_read_export_data (int fd, off_t offset, char **pbuf, size_t *plen,
		       int *perr)
{
  simple_object_read *sobj;
  const char *errmsg;
  off_t sec_offset;
  off_t sec_length;
  int found;
  char *buf;
  ssize_t c;

  *pbuf = NULL;
  *plen = 0;

  sobj = simple_object_start_read (fd, offset, RUST_EXPORT_SEGMENT_NAME,
				   &errmsg, perr);
  if (sobj == NULL)
    {
      /* If we get an error here, just pretend that we didn't find any
	 export data.  This is the right thing to do if the error is
	 that the file was not recognized as an object file.  This
	 will ignore file I/O errors, but it's not too big a deal
	 because we will wind up giving some other error later.  */
      return NULL;
    }

  found = simple_object_find_section (sobj, RUST_EXPORT_SECTION_NAME,
				      &sec_offset, &sec_length, &errmsg, perr);
  simple_object_release_read (sobj);
  if (!found)
    return errmsg;

  if (lseek (fd, offset + sec_offset, SEEK_SET) < 0)
    {
      *perr = errno;
      return _ ("lseek failed while reading export data");
    }

  buf = XNEWVEC (char, sec_length);
  if (buf == NULL)
    {
      *perr = errno;
      return _ ("memory allocation failed while reading export data");
    }

  c = read (fd, buf, sec_length);
  if (c < 0)
    {
      *perr = errno;
      free (buf);
      return _ ("read failed while reading export data");
    }

  if (c < sec_length)
    {
      free (buf);
      return _ ("short read while reading export data");
    }

  *pbuf = buf;
  *plen = sec_length;

  return NULL;
}
