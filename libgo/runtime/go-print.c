/* go-print.c -- support for the go print statement.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <stdarg.h>
#include <stdio.h>

/* This implements __go_print which is used for the print statement.
   FIXME: This is only for testing.  */

void
__go_print(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}
