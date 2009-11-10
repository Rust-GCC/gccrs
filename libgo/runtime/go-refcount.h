/* go-refcount.h -- reference count structure.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#ifndef LIBGO_GO_REFCOUNT_H
#define LIBGO_GO_REFCOUNT_H

#include "go-type.h"

/* Each entry in the reference count table looks like this.  */

struct __go_refcount_entry
{
  /* The type descriptor.  */
  const struct __go_type_descriptor *__descriptor;
  /* If the value is a pointer, or in general if the size of the value
     is the size of a pointer, this holds the value itself.  Otherwise
     it holds a pointer to the value.  This will be NULL when there is
     no value whose reference count needs adjusting.  */
  void *__value;
};

/* Each function which requires reference count adjustments has a
   local variable whose type is this structure.  */

struct __go_refcount
{
  /* A pointer to the reference count structure for the calling
     function.  This will be set to NULL if there is no caller with
     reference counts, or if all reference counts in the caller have
     been processed.  */
  struct __go_refcount *__caller;
  /* This flag is initialized to false.  It is set to true when we
     have scanned all the callers for increments.  We only need to
     scan the callers once, since we zero out each increment as we
     find it.  */
  _Bool __callers_were_scanned;
  /* This flag is initialized to false.  It is set to true if we have
     stopped scanning for decrements because there were too many stack
     frames.  When this flag is true we can not simply free newly
     allocated values.  */
  _Bool __did_not_scan_decrements;

  /* The number of entries for newly allocated values.  */
  unsigned short __decrement_new_count;
  /* The number of entries for computed values.  */
  unsigned short __decrement_computed_count;
  /* The number of entries for old values which must be
     decremented.  */
  unsigned short __decrement_old_count;
  /* The number of entries for copied values which must be
     incremented.  */
  unsigned short __increment_copy_count;

  /* The actual entries.  The number of elements in this array is the
     sum of the count variables.  */
  struct __go_refcount_entry __entries[];
};

/* The struct used to send reference count adjustments to the GC
   thread.  */

struct __go_refcount_msg
{
  /* The current epoch.  */
  unsigned int __epoch;
  /* The number of pointers whose reference counts need to be
     incremented.  */
  unsigned short __increments;
  /* The number of pointers whose reference counts need to be
     decremented.  */
  unsigned short __decrements;
  /* The pointers whose reference counts need adjusting.  The
     increment pointers come first, followed by the decrement
     pointers.  */
  void *__pointers[];
};

/* The head of the list of reference count structures.  */

extern __thread struct __go_refcount *__go_refcount_head;

extern void __go_increment_refcount (void *,
				     const struct __go_type_descriptor *);

extern void __go_refcount_flush_queue (struct __go_refcount *);

extern void __go_send_refcount_msg (struct __go_refcount_msg *);

#endif /* !defined (LIBGO_GO_REFCOUNT_H) */
