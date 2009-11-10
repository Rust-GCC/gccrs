/* go-defer.c -- manage the defer stack.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <stddef.h>

#include "go-alloc.h"

/* The defer stack is a list of these structures.  */

struct __defer_stack
{
  /* The next entry in the stack.  */
  struct __defer_stack* __next;

  /* The function to call.  */
  void (*__pfn) (void *);

  /* The argument to pass to the function.  */
  void *__arg;
};

/* This function is called each time we need to defer a call.  */

void *
__go_defer (void *stack, void (*pfn) (void *), void *arg)
{
  struct __defer_stack *n;

  n = (struct __defer_stack *) __go_alloc (sizeof (struct __defer_stack));
  n->__next = (struct __defer_stack *) stack;
  n->__pfn = pfn;
  n->__arg = arg;
  return (void *) n;
}

/* This function is called when we want to undefer the stack.  */

void
__go_undefer (void *arg)
{
  struct __defer_stack *p;

  p = (struct __defer_stack *) arg;
  while (p != NULL)
    {
      struct __defer_stack *n;

      n = p->__next;
      (*p->__pfn) (p->__arg);
      __go_free (p);
      p = n;
    }
}
