/* go-trampoline.c -- allocate a trampoline for a nested function.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>

#include "go-alloc.h"

/* In order to build a trampoline we need space which is both writable
   and executable.  We currently just allocate a whole page.  This
   needs to be more system dependent.  */

void *
__go_allocate_trampoline (size_t size)
{
  unsigned int page_size;
  void *ret;
  int i;

  page_size = getpagesize ();
  assert (page_size >= size);
  ret = __go_alloc (2 * page_size - 1);
  ret = (void *) (((uintptr_t) ret + page_size - 1)
		  & ~ ((uintptr_t) page_size - 1));
  i = mprotect (ret, size, PROT_READ | PROT_WRITE | PROT_EXEC);
  assert (i == 0);
  return ret;
}
