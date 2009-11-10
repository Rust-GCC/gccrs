/* go-byte-array-to-string.c -- convert an array of bytes to a string in Go.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include "go-alloc.h"
#include "go-string.h"

const struct __go_string *
__go_byte_array_to_string (const void* p, size_t len)
{
  const unsigned char *bytes;
  struct __go_string *ret;

  bytes = (const unsigned char *) p;
  ret = __go_alloc (sizeof (struct __go_string) + len);
  ret->__length = len;
  __builtin_memcpy (ret->__data, bytes, len);
  return ret;
}
