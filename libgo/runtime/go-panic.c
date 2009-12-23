/* go-panic.c -- support for the go panic statement.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <stdio.h>
#include <stdlib.h>

#include "go-panic.h"

/* This implements __go_panic which is used for the panic
   statement.  */

void
__go_panic ()
{
  fputs ("\npanic\n", stderr);
  abort ();
}

/* These are used by the runtime library.  */

void
__go_panic_msg (const char* msg)
{
  __go_print_msg (1, msg);
  __go_panic ();
}

void
__go_print_msg (_Bool is_panic, const char* msg)
{
  fputs (msg, is_panic ? stderr : stdout);
}
