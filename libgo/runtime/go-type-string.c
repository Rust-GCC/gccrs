/* go-type-string.c -- hash and equality string functions.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <stddef.h>

#include "go-string.h"
#include "go-type.h"

/* A string hash function for a map.  */

size_t
__go_type_hash_string (const void *vkey,
		       size_t key_size __attribute__ ((unused)))
{
  size_t ret;
  const struct __go_string *key;
  size_t len;
  size_t i;
  const unsigned char *p;

  ret = 5381;
  key = *(const struct __go_string * const *) vkey;
  if (key == NULL)
    return ret;
  len = key->__length;
  for (i = 0, p = key->__data; i < len; i++, p++)
    ret = ret * 33 + *p;
  return ret;
}

/* A string equality function for a map.  */

_Bool
__go_type_equal_string (const void *vk1, const void *vk2,
			size_t key_size __attribute__ ((unused)))
{
  const struct __go_string *k1;
  const struct __go_string *k2;

  k1 = *(const struct __go_string * const *) vk1;
  k2 = *(const struct __go_string * const *) vk2;
  if (k1 == NULL)
    return k2 == NULL || k2->__length == 0;
  else if (k2 == NULL)
    return k1->__length == 0;
  else if (k1->__length != k2->__length)
    return 0;
  else
    return __builtin_memcmp (k1->__data, k2->__data, k1->__length) == 0;
}
