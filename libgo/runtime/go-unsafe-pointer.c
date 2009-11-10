/* go-unsafe-pointer.c -- unsafe.Pointer type descriptor for Go.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <stddef.h>

#include "go-string.h"
#include "go-type.h"

/* This file provides the type descriptor for the unsafe.Pointer type.
   The unsafe package is defined by the compiler itself, which means
   that there is no package to compile to define the type
   descriptor.  */

extern const struct __go_type_descriptor unsafe_Pointer
  asm ("__go_tdn_unsafe.Pointer");

/* Used to determine the field alignment.  */
struct field_align
{
  char c;
  void *p;
};

/* The reflection string.  */
#define REFLECTION "unsafe.Pointer"
#define REFLECTION_LEN 14
static const struct
{
  size_t length;
  char data[REFLECTION_LEN];
} reflection_string =
{
  sizeof REFLECTION - 1,
  { REFLECTION }
};

static const struct __go_string * const ptr_reflection_string =
  (const struct __go_string *) &reflection_string;

const struct __go_type_descriptor unsafe_Pointer =
{
  /* __code */
  GO_UNSAFE_POINTER,
  /* __align */
  __alignof (void *),
  /* __field_align */
  offsetof (struct field_align, p) - 1,
  /* __size */
  sizeof (void *),
  /* __hash */
  __go_type_hash_identity,
  /* __equal */
  __go_type_equal_identity,
  /* __reflection */
  &ptr_reflection_string,
  /* __uncommon */
  NULL
};
