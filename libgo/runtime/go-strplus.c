/* go-strplus.c -- the go string append function.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include "go-alloc.h"
#include "go-string.h"

const struct __go_string *
__go_string_plus (const struct __go_string *s1, const struct __go_string *s2)
{
  size_t len;
  struct __go_string* ret;

  if (s1 == NULL)
    return s2;
  else if (s2 == NULL)
    return s1;

  len = s1->__length + s2->__length;
  ret = (struct __go_string *) __go_alloc (sizeof (struct __go_string) + len);
  ret->__length = len;
  __builtin_memcpy (ret->__data, s1->__data, s1->__length);
  __builtin_memcpy (ret->__data + s1->__length, s2->__data, s2->__length);
  return ret;
}
