/* go-goexit.c -- the runtime.Goexit function.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <assert.h>
#include <pthread.h>

void Goexit (void) asm ("runtime.Goexit");

void
Goexit (void)
{
  pthread_exit (NULL);
  assert (0);
}
