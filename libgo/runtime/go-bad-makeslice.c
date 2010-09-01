/* go-bad-makeslice.c -- bad call to make for a slice in Go.

   Copyright 2010 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include "go-panic.h"

extern void __go_bad_makeslice () __attribute__ ((noreturn));

void
__go_bad_makeslice ()
{
  __go_panic_msg ("slice len or cap out of range");
}
