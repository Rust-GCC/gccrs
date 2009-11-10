/* go-reflect.c -- implement unsafe.Reflect and unsafe.Typeof for Go.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <stdlib.h>
#include <stdint.h>

#include "interface.h"
#include "go-alloc.h"
#include "go-panic.h"
#include "go-string.h"
#include "go-type.h"

/* For field alignment.  */

struct field_align
{
  char c;
  struct __go_type_descriptor *p;
};

/* The type descriptors in the runtime package.  */

extern const struct __go_type_descriptor ptr_bool_descriptor
  asm ("__go_td_pN16_runtime.BoolType");
extern const struct __go_type_descriptor ptr_float32_descriptor
  asm ("__go_td_pN19_runtime.Float32Type");
extern const struct __go_type_descriptor ptr_float64_descriptor
  asm ("__go_td_pN19_runtime.Float64Type");
extern const struct __go_type_descriptor ptr_float_descriptor
  asm ("__go_td_pN17_runtime.FloatType");
extern const struct __go_type_descriptor ptr_int16_descriptor
  asm ("__go_td_pN17_runtime.Int16Type");
extern const struct __go_type_descriptor ptr_int32_descriptor
  asm ("__go_td_pN17_runtime.Int32Type");
extern const struct __go_type_descriptor ptr_int64_descriptor
  asm ("__go_td_pN17_runtime.Int64Type");
extern const struct __go_type_descriptor ptr_int8_descriptor
  asm ("__go_td_pN16_runtime.Int8Type");
extern const struct __go_type_descriptor ptr_int_descriptor
  asm ("__go_td_pN15_runtime.IntType");
extern const struct __go_type_descriptor ptr_uint16_descriptor
  asm ("__go_td_pN18_runtime.Uint16Type");
extern const struct __go_type_descriptor ptr_uint32_descriptor
  asm ("__go_td_pN18_runtime.Uint32Type");
extern const struct __go_type_descriptor ptr_uint64_descriptor
  asm ("__go_td_pN18_runtime.Uint64Type");
extern const struct __go_type_descriptor ptr_uint8_descriptor
  asm ("__go_td_pN17_runtime.Uint8Type");
extern const struct __go_type_descriptor ptr_uint_descriptor
  asm ("__go_td_pN16_runtime.UintType");
extern const struct __go_type_descriptor ptr_string_descriptor
  asm ("__go_td_pN18_runtime.StringType");
extern const struct __go_type_descriptor ptr_uintptr_descriptor
  asm ("__go_td_pN19_runtime.UintptrType");
extern const struct __go_type_descriptor ptr_dotdotdot_descriptor
  asm ("__go_td_pN21_runtime.DotDotDotType");
extern const struct __go_type_descriptor ptr_unsafe_pointer_decriptor
  asm ("__go_td_pN25_runtime.UnsafePointerType");
extern const struct __go_type_descriptor ptr_array_descriptor
  asm ("__go_td_pN17_runtime.ArrayType");
extern const struct __go_type_descriptor ptr_slice_descriptor
  asm ("__go_td_pN17_runtime.SliceType");
extern const struct __go_type_descriptor ptr_chan_descriptor
  asm ("__go_td_pN16_runtime.ChanType");
extern const struct __go_type_descriptor ptr_func_descriptor
  asm ("__go_td_pN16_runtime.FuncType");
extern const struct __go_type_descriptor ptr_interface_descriptor
  asm ("__go_td_pN21_runtime.InterfaceType");
extern const struct __go_type_descriptor ptr_map_descriptor
  asm ("__go_td_pN15_runtime.MapType");
extern const struct __go_type_descriptor ptr_ptr_descriptor
  asm ("__go_td_pN15_runtime.PtrType");
extern const struct __go_type_descriptor ptr_struct_descriptor
  asm ("__go_td_pN18_runtime.StructType");

const struct __go_type_descriptor *
get_descriptor (int code)
{
  switch (code)
    {
    case GO_BOOL:
      return &ptr_bool_descriptor;
    case GO_FLOAT32:
      return &ptr_float32_descriptor;
    case GO_FLOAT64:
      return &ptr_float64_descriptor;
    case GO_FLOAT:
      return &ptr_float_descriptor;
    case GO_INT16:
      return &ptr_int16_descriptor;
    case GO_INT32:
      return &ptr_int32_descriptor;
    case GO_INT64:
      return &ptr_int64_descriptor;
    case GO_INT8:
      return &ptr_int8_descriptor;
    case GO_INT:
      return &ptr_int_descriptor;
    case GO_UINT16:
      return &ptr_uint16_descriptor;
    case GO_UINT32:
      return &ptr_uint32_descriptor;
    case GO_UINT64:
      return &ptr_uint64_descriptor;
    case GO_UINT8:
      return &ptr_uint8_descriptor;
    case GO_UINT:
      return &ptr_uint_descriptor;
    case GO_STRING:
      return &ptr_string_descriptor;
    case GO_UINTPTR:
      return &ptr_uintptr_descriptor;
    case GO_DOTDOTDOT:
      return &ptr_dotdotdot_descriptor;
    case GO_UNSAFE_POINTER:
      return &ptr_unsafe_pointer_decriptor;
    case GO_ARRAY:
      return &ptr_array_descriptor;
    case GO_SLICE:
      return &ptr_slice_descriptor;
    case GO_CHAN:
      return &ptr_chan_descriptor;
    case GO_FUNC:
      return &ptr_func_descriptor;
    case GO_INTERFACE:
      return &ptr_interface_descriptor;
    case GO_MAP:
      return &ptr_map_descriptor;
    case GO_PTR:
      return &ptr_ptr_descriptor;
    case GO_STRUCT:
      return &ptr_struct_descriptor;
    default:
      abort ();
    }
}

/* Implement unsafe.Reflect.  */

struct reflect_ret
{
  struct __go_interface *rettype;
  void *addr;
};

struct reflect_ret Reflect (const struct __go_interface *)
  asm ("unsafe.Reflect");

struct reflect_ret
Reflect (const struct __go_interface *p)
{
  struct reflect_ret ret;

  if (p == NULL)
    {
      ret.rettype = NULL;
      ret.addr = NULL;
    }
  else
    {
      size_t size;

      ret.rettype = ((struct __go_interface *)
		     __go_alloc (sizeof (struct __go_interface)));
      ret.rettype->__type_descriptor =
	get_descriptor (p->__type_descriptor->__code);
      ret.rettype->__methods = NULL;

      /* This memcpy is really just an assignment of a const pointer
	 to a non-const pointer.  FIXME: We should canonicalize this
	 pointer, so that for a given type we always return the same
	 pointer.  */
      __builtin_memcpy (&ret.rettype->__object, &p->__type_descriptor,
			sizeof (void *));

      /* Make a copy of the value.  */
      size = p->__type_descriptor->__size;
      if (size <= sizeof (uint64_t))
	ret.addr = __go_alloc (sizeof (uint64_t));
      else
	ret.addr = __go_alloc (size);
      if (__go_is_pointer_type (p->__type_descriptor))
	*(void **) ret.addr = p->__object;
      else
	__builtin_memcpy (ret.addr, p->__object, size);
    }

  return ret;
}

/* Implement unsafe.Typeof.  */

struct __go_interface *Typeof (const struct __go_interface *)
  asm ("unsafe.Typeof");

struct __go_interface *
Typeof (const struct __go_interface *p)
{
  if (p == NULL)
    return NULL;
  else
    {
      struct __go_interface *ret;

      ret = ((struct __go_interface *)
	     __go_alloc (sizeof (struct __go_interface)));
      ret->__type_descriptor = get_descriptor (p->__type_descriptor->__code);
      ret->__methods = NULL;

      /* This memcpy is really just an assignment of a const pointer
	 to a non-const pointer.  FIXME: We should canonicalize this
	 pointer, so that for a given type we always return the same
	 pointer.  */
      __builtin_memcpy (&ret->__object, &p->__type_descriptor, sizeof (void *));

      return ret;
    }
}
