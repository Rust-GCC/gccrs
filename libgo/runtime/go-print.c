/* go-print.c -- support for the go print statement.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <stdint.h>
#include <stdio.h>

#include "array.h"
#include "go-panic.h"
#include "go-string.h"

/* This implements the various little functions which are called by
   the predeclared functions print/println/panic/panicln.  */

void
__go_print_space (_Bool is_panic)
{
  putc (' ', is_panic ? stderr : stdout);
}

void
__go_print_nl (_Bool is_panic)
{
  putc ('\n', is_panic ? stderr : stdout);
}

void
__go_print_string (_Bool is_panic, const struct __go_string* val)
{
  fprintf (is_panic ? stderr : stdout, "%.*s", (int) val->__length,
	   (const char *) &val->__data[0]);
}

void
__go_print_uint64 (_Bool is_panic, uint64_t val)
{
  fprintf (is_panic ? stderr : stdout, "%llu", (unsigned long long) val);
}

void
__go_print_int64 (_Bool is_panic, int64_t val)
{
  fprintf (is_panic ? stderr : stdout, "%lld", (long long) val);
}

void
__go_print_double (_Bool is_panic, double val)
{
  fprintf (is_panic ? stderr : stdout, "%.24g", val);
}

void
__go_print_bool (_Bool is_panic, _Bool val)
{
  fputs (val ? "true" : "false", is_panic ? stderr : stdout);
}

void
__go_print_pointer (_Bool is_panic, void *val)
{
  fprintf (is_panic ? stderr : stdout, "%p", val);
}

void
__go_print_slice (_Bool is_panic, struct __go_open_array val)
{
  fprintf (is_panic ? stderr : stdout, "[%d/%d]%p",
	   val.__count, val.__capacity, val.__values);
}
