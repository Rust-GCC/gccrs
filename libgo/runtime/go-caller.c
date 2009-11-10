/* go-caller.c -- runtime.Caller for Go.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

/* Implement runtime.Caller.  */

#include <stdint.h>

#include "go-string.h"

/* The values returned by runtime.Caller.  */

struct caller_ret
{
  uintptr_t pc;
  const struct __go_string *file;
  int line;
  _Bool ok;
};

/* Implement runtime.Caller.  */

struct caller_ret Caller (int n) asm ("runtime.Caller");

struct caller_ret
Caller (int n __attribute__ ((unused)))
{
  struct caller_ret ret;

  /* A proper implementation needs to dig through the debugging
     information.  */
  ret.pc = (uint64_t) (uintptr_t) __builtin_return_address (1);
  ret.file = NULL;
  ret.line = 0;
  ret.ok = 0;

  return ret;
}
