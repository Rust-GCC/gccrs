/* Library support for -fsplit-stack.  */
/* Copyright (C) 2009 Free Software Foundation, Inc.
   Contributed by Ian Lance Taylor <iant@google.com>.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#include "tconfig.h"
#include "tsystem.h"
#include "coretypes.h"
#include "tm.h"

/* If inhibit_libc is defined, we can not compile this file.  The
   effect is that people will not be able to use -fsplit-stack.  That
   is much better than failing the build particularly since people
   will want to define inhibit_libc while building a compiler which
   can build glibc.  */

#ifndef inhibit_libc

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/uio.h>

/* This file contains subroutines that are used by code compiled with
   -fsplit-stack.  */

/* Declare functions to avoid warnings--there is no header file for
   these internal functions.  We give these functions the flatten
   attribute in order to minimize their stack usage--here we must
   minimize stack usage even at the cost of code size, and in general
   inlining everything will do that.  */

extern void *
__generic_morestack (size_t *frame_size, void *old_stack, size_t param_size)
  __attribute__ ((no_split_stack, flatten, visibility ("hidden")));

extern void *
__generic_releasestack (size_t *pavailable)
  __attribute__ ((no_split_stack, flatten, visibility ("hidden")));

/* When we allocate a stack segment we put this header at the
   start.  */

struct stack_segment
{
  /* The previous stack segment--when a function running on this stack
     segment returns, it will run on the previous one.  */
  struct stack_segment *prev;
  /* The next stack segment, if it has been allocated--when a function
     is running on this stack segment, the next one is not being
     used.  */
  struct stack_segment *next;
  /* The total size of this stack segment.  This is zero for the
     special case of a header created to represent the initial stack
     segment--the one created when the program or thread was
     started.  */
  size_t size;
  /* The stack address when this stack was created.  This is used when
     popping the stack.  */
  void *old_stack;
};

/* The first stack segment allocated for this thread.  */

static __thread struct stack_segment *stack_segments;

/* The stack segment that we think we are currently using.  This will
   be correct in normal usage, but will be incorrect if an exception
   unwinds into a different stack segment or if longjmp jumps to a
   different stack segment.  */

static __thread struct stack_segment *current_segment;

/* Convert an integer to a decimal string without using much stack
   space.  Return a pointer to the part of the buffer to use.  We this
   instead of sprintf because sprintf will require too much stack
   space.  */

static char *
print_int (int val, char *buf, int buflen, size_t *print_len)
{
  int is_negative;
  int i;
  unsigned int uval;

  uval = (unsigned int) val;
  if (val >= 0)
    is_negative = 0;
  else
    {
      is_negative = 1;
      uval = - uval;
    }

  i = buflen;
  do
    {
      --i;
      buf[i] = '0' + (uval % 10);
      uval /= 10;
    }
  while (uval != 0 && i > 0);

  if (is_negative)
    {
      if (i > 0)
	--i;
      buf[i] = '-';
    }

  *print_len = buflen - i;
  return buf + i;
}

/* Allocate a new stack segment.  FRAME_SIZE is the required frame
   size.  */

static struct stack_segment *
allocate_segment (size_t frame_size)
{
  static unsigned int static_pagesize;
  unsigned int pagesize;
  int first;
  unsigned int overhead;
  unsigned int allocate;
  void *space;
  struct stack_segment *pss;

  pagesize = static_pagesize;
  if (pagesize == 0)
    {
      unsigned int p;

      pagesize = getpagesize ();

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
      p = __sync_val_compare_and_swap (&static_pagesize, 0, pagesize);
#else
      /* Just hope this assignment is atomic.  */
      static_pagesize = pagesize;
      p = 0;
#endif

      /* FIXME: I'm not sure this assert should be in the released
	 code.  */
      assert (p == 0 || p == pagesize);
    }

  first = current_segment == NULL;

  overhead = sizeof (struct stack_segment);
  if (first)
    overhead *= 2;

  allocate = pagesize;
  if (allocate < MINSIGSTKSZ)
    allocate = ((MINSIGSTKSZ + overhead + pagesize - 1)
		& ~ (pagesize - 1));
  if (allocate < frame_size)
    allocate = ((frame_size + overhead + pagesize - 1)
		& ~ (pagesize - 1));

  /* FIXME: If this binary requires an executable stack, then we need
     to set PROT_EXEC.  Unfortunately figuring that out is complicated
     and target dependent.  We would need to use dl_iterate_phdr to
     see if there is any object which does not have a PT_GNU_STACK
     phdr, though only for architectures which use that mechanism.  */
  space = mmap (NULL, allocate, PROT_READ | PROT_WRITE,
		MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (space == MAP_FAILED)
    {
      static const char err1[] =
	"unable to allocate additional stack space: errno ";
      char buf[24];
      static const char err2[] = "\n";
      struct iovec iov[3];
      union { char *p; const char *cp; } const_cast;

      const_cast.cp = &err1[0];
      iov[0].iov_base = const_cast.p;
      iov[0].iov_len = sizeof err1 - 1;
      /* We can't call strerror, because it may try to translate the
	 error message, and that would use too much stack space.  */
      iov[1].iov_base = print_int (errno, buf, sizeof buf, &iov[1].iov_len);
      const_cast.cp = &err2[0];
      iov[2].iov_base = const_cast.p;
      iov[2].iov_len = sizeof err2 - 1;
      /* FIXME: On systems without writev we need to issue three write
	 calls, or punt on printing errno.  For now this is irrelevant
	 since stack splitting only works on GNU/Linux anyhow.  */
      writev (2, iov, 3);
      abort ();
    }

  pss = (struct stack_segment *) space;

  if (first)
    {
      pss->prev = NULL;
      pss->next = pss + 1;
      pss->size = 0;
      stack_segments = pss;
      current_segment = pss;
      pss = pss + 1;
    }

  pss->prev = current_segment;
  pss->next = NULL;
  pss->size = allocate - overhead;
  current_segment->next = pss;

  current_segment = pss;

  return pss;
}

/* Release stack segments.  */

static void
release_segments (struct stack_segment *pss)
{
  while (pss != NULL)
    {
      struct stack_segment *next;
      unsigned int allocate;

      next = pss->next;

      if (pss->size != 0)
	allocate = pss->size + sizeof (struct stack_segment);
      else
	{
	  assert (next == pss + 1);
	  allocate = next->size = 2 * sizeof (struct stack_segment);
	  next = next->next;
	}

      if (munmap (pss, allocate) < 0)
	{
	  static const char msg[] = "munmap of stack space failed\n";
	  write (2, msg, sizeof msg - 1);
	}

      pss = next;
    }
}

/* This function is called by a processor specific function which is
   run in the prologue when more stack is needed.  The processor
   specific function handles the details of saving registers and
   frobbing the actual stack pointer.  This function is responsible
   for allocating a new stack segment and for copying a parameter
   block from the old stack to the new one.  On function entry
   *PFRAME_SIZE is the size of the required stack frame--the returned
   stack must be at least this large.  On function exit *PFRAME_SIZE
   is the amount of space remaining on the allocated stack.  OLD_STACK
   points at the parameters the old stack (really the current one
   while this function is running).  OLD_STACK is saved so that it can
   be returned by a later call to __generic_releasestack.  PARAM_SIZE
   is the size in bytes of parameters to copy to the new stack.  This
   function returns a pointer to the new stack segment, pointing to
   the memory after the parameters have been copied.  The returned
   value minus the returned *PFRAME_SIZE (or plus if the stack grows
   upward) is the first address on the stack which should not be used.

   This function is running on the old stack and has only a limited
   amount of stack space available.  */

void *
__generic_morestack (size_t *pframe_size, void *old_stack, size_t param_size)
{
  size_t frame_size = *pframe_size;
  struct stack_segment *current;

  current = current_segment;

  if (current != NULL
      && current->next != NULL
      && current->next->size < frame_size)
    {
      release_segments (current->next);
      current->next = NULL;
    }

  if (current != NULL && current->next != NULL)
    current = current->next;
  else
    current = allocate_segment (frame_size);

  current->old_stack = old_stack;

  current_segment = current;

  *pframe_size = current->size - param_size;

#ifdef STACK_GROWS_DOWNWARD
  {
    char *bottom = (char *) (current + 1) + current->size;
    __builtin_memcpy (bottom - param_size, old_stack, param_size);
    return bottom - param_size;
  }
#else
  __builtin_memcpy (current + 1, old_stack, param_size);
  return (char *) (current + 1) + param_size;
#endif
}

/* This function is called by a processor specific function when it is
   ready to release a stack segment.  This function is actually
   running on the stack segment which it should release.  So it may
   not literally free the segment, but must instead mark it for
   release at a later date.  It returns a pointer to the new stack
   segment to use, which is the one saved by a previous call to
   __generic_morestack.  The processor specific function is then
   responsible for actually updating the stack pointer.  This sets
   *PAVAILABLE to the amount of stack space now available.  */

void *
__generic_releasestack (size_t *pavailable)
{
  struct stack_segment *current;
  void *old_stack;

  current = current_segment;

  if (current->next != NULL)
    {
      release_segments (current->next);
      current->next = NULL;
    }

  old_stack = current->old_stack;
  current = current->prev;
  current_segment = current;

#ifdef STACK_GROWS_DOWNWARD
  *pavailable = (char *) old_stack - (char *) (current + 1);
#else
  *pavailable = (char *) (current + 1) + current->size - (char *) old_stack;
#endif

  return old_stack;
}

/* Pass information from the pthread_create wrapper to
   stack_split_initialize_thread.  */

struct pthread_create_args
{
  void *(*start_routine) (void *);
  void *arg;
};

/* Initialize a thread.  This is called via pthread_create.  It calls
   a target dependent function to set up any required stack guard.  */

static void* stack_split_initialize_thread (void *)
  __attribute__ ((no_split_stack));

extern void __stack_split_initialize (void)
  __attribute__ ((visibility ("hidden")));

static void *
stack_split_initialize_thread (void *varg)
{
  struct pthread_create_args *args = (struct pthread_create_args *) varg;
  void *(*start_routine) (void *);
  void *arg;

  __stack_split_initialize ();
  start_routine = args->start_routine;
  arg = args->arg;
  free (args);
  return (*start_routine) (arg);
}

/* This function wraps calls to pthread_create to make sure that the
   stack guard is initialized for new threads.  FIXME: This hack will
   not be necessary if glibc supports -fsplit-stack directly.  */

int __wrap_pthread_create (pthread_t *, const pthread_attr_t *,
			   void *(*start_routine) (void *), void *)
  __attribute__ ((visibility ("hidden")));

extern int __real_pthread_create (pthread_t *, const pthread_attr_t *,
				  void *(*start_routine) (void *), void *)
  __attribute__ ((weak));

int
__wrap_pthread_create (pthread_t *tid, const pthread_attr_t *attr,
		       void *(*start_routine) (void *), void *arg)
{
  struct pthread_create_args* args;

  args = malloc (sizeof (struct pthread_create_args));
  if (args == NULL)
    return EAGAIN;
  args->start_routine = start_routine;
  args->arg = arg;
  return __real_pthread_create (tid, attr, stack_split_initialize_thread, args);
}

#endif /* !defined (inhibit_libc) */
