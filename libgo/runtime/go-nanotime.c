// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// Return time in nanoseconds.  This is only used for computing runtime.

#include <assert.h>
#include <sys/time.h>

#include "runtime.h"

int64
nanotime (void)
{
  int i;
  struct timeval tv;

  i = gettimeofday (&tv, NULL);
  assert (i == 0);

  return (int64) tv.tv_sec * 1000000000 + (int64) tv.tv_usec * 1000;
}
