/* go-bad-index.c -- bad array/string index error in Go.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include "go-panic.h"

extern void __go_bad_index () __attribute__ ((noreturn));

void
__go_bad_index ()
{
  __go_panic ("index out of range");
}
