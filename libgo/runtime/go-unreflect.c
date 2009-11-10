/* go-unreflect.c -- implement unsafe.Unreflect for Go.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include "go-alloc.h"
#include "go-type.h"
#include "interface.h"

/* Implement unsafe.Unreflect.  */

struct __go_interface *Unreflect (const struct __go_interface *type,
				  void *object)
  asm ("unsafe.Unreflect");

struct __go_interface *
Unreflect (const struct __go_interface *type, void *object)
{
  struct __go_interface *ret;

  ret = (struct __go_interface *) __go_alloc (sizeof (struct __go_interface));
  ret->__type_descriptor = type->__object;
  ret->__methods = NULL;
  // FIXME: Handle reference counts.
  if (__go_is_pointer_type (ret->__type_descriptor))
    ret->__object = *(void **) object;
  else
    ret->__object = object;
  return ret;
}
