/* go-new-interface-pointer.c -- make a new interface from a pointer value.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <assert.h>

#include "go-alloc.h"
#include "go-refcount.h"
#include "interface.h"

/* Allocate a new interface for a type which is represented as a
   pointer.  OBJECT is the value to be stored in the interface.  */

struct __go_interface *
__go_new_interface_pointer (const struct __go_type_descriptor *type_descriptor,
			    void *methods, void *object)
{
  struct __go_interface *ret;

  assert (__go_is_pointer_type (type_descriptor));
  ret = __go_alloc (sizeof (struct __go_interface));
  ret->__type_descriptor = type_descriptor;
  ret->__methods = methods;
  ret->__object = object;

  __go_increment_refcount (object, type_descriptor);

  return ret;
}
