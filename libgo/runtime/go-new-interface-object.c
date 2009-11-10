/* go-new-interface-object.c -- make a new interface from a non-pointer.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <assert.h>

#include "go-alloc.h"
#include "interface.h"

/* Allocate a new interface for a type which is not represented as a
   pointer.  OBJECT points to the value.  */

struct __go_interface *
__go_new_interface_object (const struct __go_type_descriptor *type_descriptor,
			   void *methods, size_t object_size,
			   const void *object)
{
  struct __go_interface *ret;

  assert (!__go_is_pointer_type (type_descriptor));
  assert (object_size == type_descriptor->__size);
  ret = __go_alloc (sizeof (struct __go_interface));
  ret->__type_descriptor = type_descriptor;
  ret->__methods = methods;
  ret->__object = __go_alloc (object_size);
  /* FIXME: Set reference count.  */
  __builtin_memcpy (ret->__object, object, object_size);
  return ret;
}
