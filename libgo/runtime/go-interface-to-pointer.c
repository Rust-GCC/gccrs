/* go-interface-to-pointer.c -- get a pointer from an interface.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <assert.h>

#include "go-panic.h"
#include "interface.h"

/* Get a pointer from an interface.  This checks that the types match,
   and crashes if they don't.  */

void *
__go_interface_to_pointer (const struct __go_type_descriptor *lhs_descriptor,
			   const void *rhs_arg)
{
  const struct __go_interface *rhs = (const struct __go_interface *) rhs_arg;

  assert (__go_is_pointer_type (lhs_descriptor));

  if (rhs == NULL)
    return NULL;

  if (rhs->__type_descriptor != lhs_descriptor
      && !__go_type_descriptors_equal (rhs->__type_descriptor, lhs_descriptor)
      && lhs_descriptor->__code != GO_UNSAFE_POINTER
      && rhs->__type_descriptor->__code != GO_UNSAFE_POINTER)
    {
      struct __go_interface *panic_arg;

      newTypeAssertionError(NULL, rhs->__type_descriptor, lhs_descriptor,
			    NULL, rhs->__type_descriptor->__reflection,
			    lhs_descriptor->__reflection, NULL, &panic_arg);
      __go_panic(panic_arg);
    }
  return rhs->__object;
}
