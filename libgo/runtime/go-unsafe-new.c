/* go-unsafe-new.c -- unsafe.New function for Go.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include "go-alloc.h"
#include "go-type.h"
#include "interface.h"

/* Implement unsafe.New.  */

void *New (const struct __go_interface *type) asm ("libgo_unsafe.unsafe.New");

/* The dynamic type of the argument will be a pointer to a type
   descriptor.  */

void *
New (const struct __go_interface *type)
{
  const void *object;
  const struct __go_type_descriptor *descriptor;

  object = type->__object;
  descriptor = (const struct __go_type_descriptor *) object;
  return __go_alloc (descriptor->__size);
}
