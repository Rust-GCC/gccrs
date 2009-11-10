/* go-interface-compare.c -- compare two interface values.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "interface.h"

/* Compare two interface values.  Return 0 for equal, not zero for not
   equal (return value is like strcmp).  */

int
__go_interface_compare (const void *left_arg, const void* right_arg)
{
  const struct __go_interface *left =
    (const struct __go_interface *) left_arg;
  const struct __go_interface *right =
    (const struct __go_interface *) right_arg;

  if (left == NULL && right == NULL)
    return 0;
  if (left == NULL || right == NULL)
    return 1;
  if (!__go_type_descriptors_equal (left->__type_descriptor,
				    right->__type_descriptor))
    return 1;
  if (__go_is_pointer_type (left->__type_descriptor))
    return left->__object == right->__object ? 0 : 1;
  if (!left->__type_descriptor->__equal (left->__object, right->__object,
					 left->__type_descriptor->__size))
    return 1;
  return 0;
}
