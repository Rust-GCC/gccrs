/* go-string-range.c -- implement a range clause over a string.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <stddef.h>

#include "go-string.h"

/* Iterate over a string.  This takes a pointer to the index and a
   pointer to the value, both ints.  The index is the index of the
   next Unicode character to read; it is updated.  If the pointer to
   the value is not NULL, the value is set to that character.  This
   returns true if it got another character, false otherwise.  */

_Bool
__go_string_range (const struct __go_string* str, int* pindex, int* pvalue)
{
  size_t index;
  size_t length;
  int value;
  int add;

  index = (size_t) *pindex;

  length = str->__length;
  if (index >= length)
    return 0;

  if (pvalue == NULL)
    pvalue = &value;

  add = __go_get_rune (str->__data + index, length - index, pvalue);

  if (add > 0)
    *pindex += add;
  else
    *pindex = (int) length;

  return 1;
}
