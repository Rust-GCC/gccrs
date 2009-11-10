/* interface.h -- the interface type for Go.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <stddef.h>
#include <stdint.h>

#include "go-type.h"

/* An variable with an interface type is represented as a pointer to
   this struct.  */

struct __go_interface
{
  /* A pointer to the type descriptor for the dynamic type of the
     object.  */
  const struct __go_type_descriptor *__type_descriptor;

  /* A pointer to the methods for the interface.  This is effectively
     the vtable for this interface.  This is simply a list of pointers
     to functions.  They are in the same order as the list in the
     internal representation of the interface, which sorts them by
     name.  */
  const void* __methods;

  /* The object.  If the object is a pointer--if the type descriptor
     code is GO_PTR or GO_UNSAFE_POINTER--then this field is the value
     of the object itself.  Otherwise this is a pointer to memory
     which holds the value.  */
  void *__object;
};

extern struct __go_interface *
__go_new_interface_pointer (const struct __go_type_descriptor *descriptor,
			    void *methods, void *object);

extern struct __go_interface *
__go_new_interface_object (const struct __go_type_descriptor *descriptor,
			   void *methods, size_t object_size,
			   const void* object);

extern struct __go_interface *
__go_convert_interface (const struct __go_type_descriptor *,
			const void *rhs,
			_Bool *success);
