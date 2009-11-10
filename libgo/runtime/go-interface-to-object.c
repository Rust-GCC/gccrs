/* go-interface-to-object.c -- get an object from an interface.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <assert.h>
#include <stdlib.h>

#include "interface.h"

/* Get an object from an interface.  This checks that the types match,
   and crashes if they don't.  */

void
__go_interface_to_object (void *result,
			  const struct __go_type_descriptor *lhs_descriptor,
			  size_t object_size, const void *rhs_arg)
{
  const struct __go_interface *rhs = (const struct __go_interface *) rhs_arg;

  assert (!__go_is_pointer_type (lhs_descriptor));
  if ((rhs->__type_descriptor != lhs_descriptor
       && !__go_type_descriptors_equal (rhs->__type_descriptor,
					lhs_descriptor))
      || rhs->__type_descriptor->__size != object_size)
    abort ();
  __builtin_memcpy (result, rhs->__object, object_size);
}
