/* go-go.c -- the go function.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <assert.h>
#include <stdlib.h>
#include <pthread.h>

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
