/* go-refcount-msg.c -- send reference counts to GC thread.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <stdio.h>
#include <stdlib.h>

#include "go-alloc.h"
#include "go-refcount.h"

/* Send a reference count message to the GC thread.  */

void
__go_send_refcount_msg (struct __go_refcount_msg *msg)
{
  static _Bool init;
  static _Bool debug;

  if (!init)
    {
      debug = getenv ("GO_DEBUG_REFCOUNT") != NULL;
      init = 1;
    }

  if (debug)
    {
      unsigned int count;
      unsigned int i;
      unsigned int offset;

      count = msg->__increments;
      for (i = 0; i < count; ++i)
	fprintf (stderr, "Increment refcount %p\n", msg->__pointers[i]);
      offset = count;
      count = msg->__decrements;
      for (i = 0; i < count; ++i)
	fprintf (stderr, "Decrement refcount %p\n",
		 msg->__pointers[i + offset]);
    }

  /* FIXME: Not implemented.  */

  __go_free (msg);
}
