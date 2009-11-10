/* go-type-interface.c -- hash and equality interface functions.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <assert.h>
#include <stddef.h>

#include "interface.h"
#include "go-type.h"

/* A hash function for an interface.  */

size_t
__go_type_hash_interface (const void *vval,
			  size_t key_size __attribute__ ((unused)))
{
  const struct __go_interface *val;
  size_t size;

  val = *(const struct __go_interface * const *) vval;
  if (val == NULL)
    return 0;
  size = val->__type_descriptor->__size;
  if (__go_is_pointer_type (val->__type_descriptor))
    return val->__type_descriptor->__hash (&val->__object, size);
  else
    return val->__type_descriptor->__hash (val->__object, size);
}

/* An equality function for an interface.  */

_Bool
__go_type_equal_interface (const void *vv1, const void *vv2,
			   size_t key_size __attribute__ ((unused)))
{
  const struct __go_interface *v1;
  const struct __go_interface *v2;

  v1 = *(const struct __go_interface * const *) vv1;
  v2 = *(const struct __go_interface * const *) vv2;
  if (v1 == NULL || v2 == NULL)
    return v1 == v2;
  if (!__go_type_descriptors_equal (v1->__type_descriptor,
				    v2->__type_descriptor))
    return 0;
  if (__go_is_pointer_type (v1->__type_descriptor))
    return v1->__object == v2->__object;
  else
    return v1->__type_descriptor->__equal (v1->__object, v2->__object,
					   v1->__type_descriptor->__size);
}
