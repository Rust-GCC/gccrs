/* go-go.c -- the go function.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <pthread.h>

#include "config.h"
#include "go-panic.h"
#include "go-alloc.h"
#include "runtime.h"

#undef int /* FIXME */

/* What to call.  */

struct call
{
  void (*pfn) (void *);
  void *arg;
  struct M *m;
};

/* Start the thread.  */

static void *
start_go_thread (void *thread_arg)
{
  struct call *pc = (struct call *) thread_arg;
  void (*pfn) (void *);
  void *arg;

#ifdef __rtems__
  __wrap_rtems_task_variable_add ((void **) &m);
  __wrap_rtems_task_variable_add ((void **) &__go_panic_defer);
#endif

  pfn = pc->pfn;
  arg = pc->arg;
  m = pc->m;
  free (pc);

  (*pfn) (arg);

  return NULL;
}

/* Implement the go statement.  */

void
__go_go (void (*pfn) (void*), void* arg)
{
  int i;
  pthread_attr_t attr;
  struct call *pc;

  i = pthread_attr_init (&attr);
  assert (i == 0);
  i = pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
  assert (i == 0);

#ifdef LINKER_SUPPORTS_SPLIT_STACK
  /* The linker knows how to handle calls between code which uses
     -fsplit-stack and code which does not.  That means that we can
     run with a smaller stack and rely on the -fsplit-stack support to
     save us.  The GNU/Linux glibc library won't let us have a very
     small stack, but we make it as small as we can.  */
#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN 8192
#endif
  i = pthread_attr_setstacksize (&attr, PTHREAD_STACK_MIN);
  assert (i == 0);
#endif

  pc = malloc (sizeof (struct call));
  pc->pfn = pfn;
  pc->arg = arg;
  pc->m = __go_alloc (sizeof (M));
  __builtin_memset (pc->m, 0, sizeof (M));
  pc->m->mcache = allocmcache ();

  pthread_t tid;
  i = pthread_create (&tid, &attr, start_go_thread, pc);
  assert (i == 0);

  i = pthread_attr_destroy (&attr);
  assert (i == 0);
}
