/* go-strcmp.c -- the go string comparison function.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include "go-string.h"

int
__go_strcmp(const struct __go_string* s1, const struct __go_string* s2)
{
  int i;

  if (s1 == NULL)
    {
      if (s2 == NULL || s2->__length == 0)
	return 0;
      return -1;
    }
  else if (s2 == NULL)
    {
      if (s1->__length == 0)
	return 0;
      return 1;
    }

  i = __builtin_memcmp(s1->__data, s2->__data,
		       (s1->__length < s2->__length
			? s1->__length
			: s2->__length));
  if (i != 0)
    return i;

  if (s1->__length < s2->__length)
    return -1;
  else if (s1->__length > s2->__length)
    return 1;
  else
    return 0;
}
