/* go-panic.c -- support for the go panic statement.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "go-panic.h"

/* This implements __go_panic which is used for the panic statement.
   FIXME: This is only for testing.  */

void
__go_panic(const char* format, ...)
{
  if (*format == '\0')
    fprintf(stderr, "panic");
  else
    {
      fprintf(stderr, "panic: ");

      va_list args;
      va_start(args, format);
      vfprintf(stderr, format, args);
      va_end(args);
    }

  fputc('\n', stderr);

  abort();
}
