/* go-strslice.c -- the go string slice function.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include "go-alloc.h"
#include "go-string.h"
#include "go-panic.h"

const struct __go_string*
__go_string_slice (const struct __go_string *s, size_t start, size_t end)
{
  size_t len;
  struct __go_string *ret;

  len = s == NULL ? 0 : s->__length;
  if (end == (size_t) -1)
    end = len;
  if (start > len || end < start || end > len)
    __go_panic("string index out of bounds");
  if (s == NULL)
    return NULL;
  len = end - start;
  ret = (struct __go_string *) __go_alloc (sizeof (struct __go_string) + len);
  ret->__length = len;
  __builtin_memcpy (ret->__data, s->__data + start, len);
  return ret;
}
