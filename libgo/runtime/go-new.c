/* go-new.c -- the generic go new() function.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include "go-alloc.h"

void *
__go_new (size_t size)
{
  void *ret;

  ret = __go_alloc (size);
  __builtin_memset (ret, 0, size);
  return ret;
}
