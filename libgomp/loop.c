/* Copyright (C) 2005-2019 Free Software Foundation, Inc.
   Contributed by Richard Henderson <rth@redhat.com>.

   This file is part of the GNU Offloading and Multi Processing Library
   (libgomp).

   Libgomp is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   Libgomp is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

/* This file handles the LOOP (FOR/DO) construct.  */

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "libgomp.h"


ialias (GOMP_loop_runtime_next)
ialias_redirect (GOMP_taskgroup_reduction_register)

/* Initialize the given work share construct from the given arguments.  */

static inline void
gomp_loop_init (struct gomp_work_share *ws, long start, long end, long incr,
		enum gomp_schedule_type sched, long chunk_size)
{
  ws->sched = sched;
  ws->chunk_size = chunk_size;
  /* Canonicalize loops that have zero iterations to ->next == ->end.  */
  ws->end = ((incr > 0 && start > end) || (incr < 0 && start < end))
	    ? start : end;
  ws->incr = incr;
  ws->next = start;
  if (sched == GFS_DYNAMIC)
    {
      ws->chunk_size *= incr;

#ifdef HAVE_SYNC_BUILTINS
      {
	/* For dynamic scheduling prepare things to make each iteration
	   faster.  */
	struct gomp_thread *thr = gomp_thread ();
	struct gomp_team *team = thr->ts.team;
	long nthreads = team ? team->nthreads : 1;

	if (__builtin_expect (incr > 0, 1))
	  {
	    /* Cheap overflow protection.  */
	    if (__builtin_expect ((nthreads | ws->chunk_size)
				  >= 1UL << (sizeof (long)
					     * __CHAR_BIT__ / 2 - 1), 0))
	      ws->mode = 0;
	    else
	      ws->mode = ws->end < (LONG_MAX
				    - (nthreads + 1) * ws->chunk_size);
	  }
	/* Cheap overflow protection.  */
	else if (__builtin_expect ((nthreads | -ws->chunk_size)
				   >= 1UL << (sizeof (long)
					      * __CHAR_BIT__ / 2 - 1), 0))
	  ws->mode = 0;
	else
	  ws->mode = ws->end > (nthreads + 1) * -ws->chunk_size - LONG_MAX;
      }
#endif
    }
}

/* The *_start routines are called when first encountering a loop construct
   that is not bound directly to a parallel construct.  The first thread
   that arrives will create the work-share construct; subsequent threads
   will see the construct exists and allocate work from it.

   START, END, INCR are the bounds of the loop; due to the restrictions of
   OpenMP, these values must be the same in every thread.  This is not
   verified (nor is it entirely verifiable, since START is not necessarily
   retained intact in the work-share data structure).  CHUNK_SIZE is the
   scheduling parameter; again this must be identical in all threads.

   Returns true if there's any work for this thread to perform.  If so,
   *ISTART and *IEND are filled with the bounds of the iteration block
   allocated to this thread.  Returns false if all work was assigned to
   other threads prior to this thread's arrival.  */

static bool
gomp_loop_static_start (long start, long end, long incr, long chunk_size,
			long *istart, long *iend)
{
  struct gomp_thread *thr = gomp_thread ();

  thr->ts.static_trip = 0;
  if (gomp_work_share_start (0))
    {
      gomp_loop_init (thr->ts.work_share, start, end, incr,
		      GFS_STATIC, chunk_size);
      gomp_work_share_init_done ();
    }

  return !gomp_iter_static_next (istart, iend);
}

/* The current dynamic implementation is always monotonic.  The
   entrypoints without nonmonotonic in them have to be always monotonic,
   but the nonmonotonic ones could be changed to use work-stealing for
   improved scalability.  */

static bool
gomp_loop_dynamic_start (long start, long end, long incr, long chunk_size,
			 long *istart, long *iend)
{
  struct gomp_thread *thr = gomp_thread ();
  bool ret;

  if (gomp_work_share_start (0))
    {
      gomp_loop_init (thr->ts.work_share, start, end, incr,
		      GFS_DYNAMIC, chunk_size);
      gomp_work_share_init_done ();
    }

#ifdef HAVE_SYNC_BUILTINS
  ret = gomp_iter_dynamic_next (istart, iend);
#else
  gomp_mutex_lock (&thr->ts.work_share->lock);
  ret = gomp_iter_dynamic_next_locked (istart, iend);
  gomp_mutex_unlock (&thr->ts.work_share->lock);
#endif

  return ret;
}

/* Similarly as for dynamic, though the question is how can the chunk sizes
   be decreased without a central locking or atomics.  */

static bool
gomp_loop_guided_start (long start, long end, long incr, long chunk_size,
			long *istart, long *iend)
{
  struct gomp_thread *thr = gomp_thread ();
  bool ret;

  if (gomp_work_share_start (0))
    {
      gomp_loop_init (thr->ts.work_share, start, end, incr,
		      GFS_GUIDED, chunk_size);
      gomp_work_share_init_done ();
    }

#ifdef HAVE_SYNC_BUILTINS
  ret = gomp_iter_guided_next (istart, iend);
#else
  gomp_mutex_lock (&thr->ts.work_share->lock);
  ret = gomp_iter_guided_next_locked (istart, iend);
  gomp_mutex_unlock (&thr->ts.work_share->lock);
#endif

  return ret;
}

bool
GOMP_loop_runtime_start (long start, long end, long incr,
			 long *istart, long *iend)
{
  struct gomp_task_icv *icv = gomp_icv (false);
  switch (icv->run_sched_var & ~GFS_MONOTONIC)
    {
    case GFS_STATIC:
      return gomp_loop_static_start (start, end, incr,
				     icv->run_sched_chunk_size,
				     istart, iend);
    case GFS_DYNAMIC:
      return gomp_loop_dynamic_start (start, end, incr,
				      icv->run_sched_chunk_size,
				      istart, iend);
    case GFS_GUIDED:
      return gomp_loop_guided_start (start, end, incr,
				     icv->run_sched_chunk_size,
				     istart, iend);
    case GFS_AUTO:
      /* For now map to schedule(static), later on we could play with feedback
	 driven choice.  */
      return gomp_loop_static_start (start, end, incr, 0, istart, iend);
    default:
      abort ();
    }
}

static long
gomp_adjust_sched (long sched, long *chunk_size)
{
  sched &= ~GFS_MONOTONIC;
  switch (sched)
    {
    case GFS_STATIC:
    case GFS_DYNAMIC:
    case GFS_GUIDED:
      return sched;
    /* GFS_RUNTIME is used for runtime schedule without monotonic
       or nonmonotonic modifiers on the clause.
       GFS_RUNTIME|GFS_MONOTONIC for runtime schedule with monotonic
       modifier.  */
    case GFS_RUNTIME:
    /* GFS_AUTO is used for runtime schedule with nonmonotonic
       modifier.  */
    case GFS_AUTO:
      {
	struct gomp_task_icv *icv = gomp_icv (false);
	sched = icv->run_sched_var & ~GFS_MONOTONIC;
	switch (sched)
	  {
	  case GFS_STATIC:
	  case GFS_DYNAMIC:
	  case GFS_GUIDED:
	    *chunk_size = icv->run_sched_chunk_size;
	    break;
	  case GFS_AUTO:
	    sched = GFS_STATIC;
	    *chunk_size = 0;
	    break;
	  default:
	    abort ();
	  }
	return sched;
      }
    default:
      abort ();
    }
}

bool
GOMP_loop_start (long start, long end, long incr, long sched,
		 long chunk_size, long *istart, long *iend,
		 uintptr_t *reductions, void **mem)
{
  struct gomp_thread *thr = gomp_thread ();

  thr->ts.static_trip = 0;
  if (reductions)
    gomp_workshare_taskgroup_start ();
  if (gomp_work_share_start (0))
    {
      sched = gomp_adjust_sched (sched, &chunk_size);
      gomp_loop_init (thr->ts.work_share, start, end, incr,
		      sched, chunk_size);
      if (reductions)
	{
	  GOMP_taskgroup_reduction_register (reductions);
	  thr->task->taskgroup->workshare = true;
	  thr->ts.work_share->task_reductions = reductions;
	}
      if (mem)
	{
	  uintptr_t size = (uintptr_t) *mem;
#define INLINE_ORDERED_TEAM_IDS_OFF \
  ((offsetof (struct gomp_work_share, inline_ordered_team_ids)		\
    + __alignof__ (long long) - 1) & ~(__alignof__ (long long) - 1))
	  if (size > (sizeof (struct gomp_work_share)
		      - INLINE_ORDERED_TEAM_IDS_OFF))
	    *mem
	      = (void *) (thr->ts.work_share->ordered_team_ids
			  = gomp_malloc_cleared (size));
	  else
	    *mem = memset (((char *) thr->ts.work_share)
			   + INLINE_ORDERED_TEAM_IDS_OFF, '\0', size);
	}
      gomp_work_share_init_done ();
    }
  else
    {
      if (reductions)
	{
	  uintptr_t *first_reductions = thr->ts.work_share->task_reductions;
	  gomp_workshare_task_reduction_register (reductions,
						  first_reductions);
	}
      if (mem)
	{
	  if ((offsetof (struct gomp_work_share, inline_ordered_team_ids)
	       & (__alignof__ (long long) - 1)) == 0)
	    *mem = (void *) thr->ts.work_share->ordered_team_ids;
	  else
	    {
	      uintptr_t p = (uintptr_t) thr->ts.work_share->ordered_team_ids;
	      p += __alignof__ (long long) - 1;
	      p &= ~(__alignof__ (long long) - 1);
	      *mem = (void *) p;
	    }
	}
    }

  if (!istart)
    return true;
  return ialias_call (GOMP_loop_runtime_next) (istart, iend);
}

/* The *_ordered_*_start routines are similar.  The only difference is that
   this work-share construct is initialized to expect an ORDERED section.  */

static bool
gomp_loop_ordered_static_start (long start, long end, long incr,
				long chunk_size, long *istart, long *iend)
{
  struct gomp_thread *thr = gomp_thread ();

  thr->ts.static_trip = 0;
  if (gomp_work_share_start (1))
    {
      gomp_loop_init (thr->ts.work_share, start, end, incr,
		      GFS_STATIC, chunk_size);
      gomp_ordered_static_init ();
      gomp_work_share_init_done ();
    }

  return !gomp_iter_static_next (istart, iend);
}

static bool
gomp_loop_ordered_dynamic_start (long start, long end, long incr,
				 long chunk_size, long *istart, long *iend)
{
  struct gomp_thread *thr = gomp_thread ();
  bool ret;

  if (gomp_work_share_start (1))
    {
      gomp_loop_init (thr->ts.work_share, start, end, incr,
		      GFS_DYNAMIC, chunk_size);
      gomp_mutex_lock (&thr->ts.work_share->lock);
      gomp_work_share_init_done ();
    }
  else
    gomp_mutex_lock (&thr->ts.work_share->lock);

  ret = gomp_iter_dynamic_next_locked (istart, iend);
  if (ret)
    gomp_ordered_first ();
  gomp_mutex_unlock (&thr->ts.work_share->lock);

  return ret;
}

static bool
gomp_loop_ordered_guided_start (long start, long end, long incr,
				long chunk_size, long *istart, long *iend)
{
  struct gomp_thread *thr = gomp_thread ();
  bool ret;

  if (gomp_work_share_start (1))
    {
      gomp_loop_init (thr->ts.work_share, start, end, incr,
		      GFS_GUIDED, chunk_size);
      gomp_mutex_lock (&thr->ts.work_share->lock);
      gomp_work_share_init_done ();
    }
  else
    gomp_mutex_lock (&thr->ts.work_share->lock);

  ret = gomp_iter_guided_next_locked (istart, iend);
  if (ret)
    gomp_ordered_first ();
  gomp_mutex_unlock (&thr->ts.work_share->lock);

  return ret;
}

bool
GOMP_loop_ordered_runtime_start (long start, long end, long incr,
				 long *istart, long *iend)
{
  struct gomp_task_icv *icv = gomp_icv (false);
  switch (icv->run_sched_var & ~GFS_MONOTONIC)
    {
    case GFS_STATIC:
      return gomp_loop_ordered_static_start (start, end, incr,
					     icv->run_sched_chunk_size,
					     istart, iend);
    case GFS_DYNAMIC:
      return gomp_loop_ordered_dynamic_start (start, end, incr,
					      icv->run_sched_chunk_size,
					      istart, iend);
    case GFS_GUIDED:
      return gomp_loop_ordered_guided_start (start, end, incr,
					     icv->run_sched_chunk_size,
					     istart, iend);
    case GFS_AUTO:
      /* For now map to schedule(static), later on we could play with feedback
	 driven choice.  */
      return gomp_loop_ordered_static_start (start, end, incr,
					     0, istart, iend);
    default:
      abort ();
    }
}

bool
GOMP_loop_ordered_start (long start, long end, long incr, long sched,
			 long chunk_size, long *istart, long *iend,
			 uintptr_t *reductions, void **mem)
{
  struct gomp_thread *thr = gomp_thread ();
  size_t ordered = 1;
  bool ret;

  thr->ts.static_trip = 0;
  if (reductions)
    gomp_workshare_taskgroup_start ();
  if (mem)
    ordered += (uintptr_t) *mem;
  if (gomp_work_share_start (ordered))
    {
      sched = gomp_adjust_sched (sched, &chunk_size);
      gomp_loop_init (thr->ts.work_share, start, end, incr,
		      sched, chunk_size);
      if (reductions)
	{
	  GOMP_taskgroup_reduction_register (reductions);
	  thr->task->taskgroup->workshare = true;
	  thr->ts.work_share->task_reductions = reductions;
	}
      if (sched == GFS_STATIC)
	gomp_ordered_static_init ();
      else
	gomp_mutex_lock (&thr->ts.work_share->lock);
      gomp_work_share_init_done ();
    }
  else
    {
      if (reductions)
	{
	  uintptr_t *first_reductions = thr->ts.work_share->task_reductions;
	  gomp_workshare_task_reduction_register (reductions,
						  first_reductions);
	}
      sched = thr->ts.work_share->sched;
      if (sched != GFS_STATIC)
	gomp_mutex_lock (&thr->ts.work_share->lock);
    }

  if (mem)
    {
      uintptr_t p
	= (uintptr_t) (thr->ts.work_share->ordered_team_ids
		       + (thr->ts.team ? thr->ts.team->nthreads : 1));
      p += __alignof__ (long long) - 1;
      p &= ~(__alignof__ (long long) - 1);
      *mem = (void *) p;
    }

  switch (sched)
    {
    case GFS_STATIC:
    case GFS_AUTO:
      return !gomp_iter_static_next (istart, iend);
    case GFS_DYNAMIC:
      ret = gomp_iter_dynamic_next_locked (istart, iend);
      break;
    case GFS_GUIDED:
      ret = gomp_iter_guided_next_locked (istart, iend);
      break;
    default:
      abort ();
    }

  if (ret)
    gomp_ordered_first ();
  gomp_mutex_unlock (&thr->ts.work_share->lock);
  return ret;
}

/* The *_doacross_*_start routines are similar.  The only difference is that
   this work-share construct is initialized to expect an ORDERED(N) - DOACROSS
   section, and the worksharing loop iterates always from 0 to COUNTS[0] - 1
   and other COUNTS array elements tell the library number of iterations
   in the ordered inner loops.  */

static bool
gomp_loop_doacross_static_start (unsigned ncounts, long *counts,
				 long chunk_size, long *istart, long *iend)
{
  struct gomp_thread *thr = gomp_thread ();

  thr->ts.static_trip = 0;
  if (gomp_work_share_start (0))
    {
      gomp_loop_init (thr->ts.work_share, 0, counts[0], 1,
		      GFS_STATIC, chunk_size);
      gomp_doacross_init (ncounts, counts, chunk_size, 0);
      gomp_work_share_init_done ();
    }

  return !gomp_iter_static_next (istart, iend);
}

static bool
gomp_loop_doacross_dynamic_start (unsigned ncounts, long *counts,
				  long chunk_size, long *istart, long *iend)
{
  struct gomp_thread *thr = gomp_thread ();
  bool ret;

  if (gomp_work_share_start (0))
    {
      gomp_loop_init (thr->ts.work_share, 0, counts[0], 1,
		      GFS_DYNAMIC, chunk_size);
      gomp_doacross_init (ncounts, counts, chunk_size, 0);
      gomp_work_share_init_done ();
    }

#ifdef HAVE_SYNC_BUILTINS
  ret = gomp_iter_dynamic_next (istart, iend);
#else
  gomp_mutex_lock (&thr->ts.work_share->lock);
  ret = gomp_iter_dynamic_next_locked (istart, iend);
  gomp_mutex_unlock (&thr->ts.work_share->lock);
#endif

  return ret;
}

static bool
gomp_loop_doacross_guided_start (unsigned ncounts, long *counts,
				 long chunk_size, long *istart, long *iend)
{
  struct gomp_thread *thr = gomp_thread ();
  bool ret;

  if (gomp_work_share_start (0))
    {
      gomp_loop_init (thr->ts.work_share, 0, counts[0], 1,
		      GFS_GUIDED, chunk_size);
      gomp_doacross_init (ncounts, counts, chunk_size, 0);
      gomp_work_share_init_done ();
    }

#ifdef HAVE_SYNC_BUILTINS
  ret = gomp_iter_guided_next (istart, iend);
#else
  gomp_mutex_lock (&thr->ts.work_share->lock);
  ret = gomp_iter_guided_next_locked (istart, iend);
  gomp_mutex_unlock (&thr->ts.work_share->lock);
#endif

  return ret;
}

bool
GOMP_loop_doacross_runtime_start (unsigned ncounts, long *counts,
				  long *istart, long *iend)
{
  struct gomp_task_icv *icv = gomp_icv (false);
  switch (icv->run_sched_var & ~GFS_MONOTONIC)
    {
    case GFS_STATIC:
      return gomp_loop_doacross_static_start (ncounts, counts,
					      icv->run_sched_chunk_size,
					      istart, iend);
    case GFS_DYNAMIC:
      return gomp_loop_doacross_dynamic_start (ncounts, counts,
					       icv->run_sched_chunk_size,
					       istart, iend);
    case GFS_GUIDED:
      return gomp_loop_doacross_guided_start (ncounts, counts,
					      icv->run_sched_chunk_size,
					      istart, iend);
    case GFS_AUTO:
      /* For now map to schedule(static), later on we could play with feedback
	 driven choice.  */
      return gomp_loop_doacross_static_start (ncounts, counts,
					      0, istart, iend);
    default:
      abort ();
    }
}

bool
GOMP_loop_doacross_start (unsigned ncounts, long *counts, long sched,
			  long chunk_size, long *istart, long *iend,
			  uintptr_t *reductions, void **mem)
{
  struct gomp_thread *thr = gomp_thread ();

  thr->ts.static_trip = 0;
  if (reductions)
    gomp_workshare_taskgroup_start ();
  if (gomp_work_share_start (0))
    {
      size_t extra = 0;
      if (mem)
	extra = (uintptr_t) *mem;
      sched = gomp_adjust_sched (sched, &chunk_size);
      gomp_loop_init (thr->ts.work_share, 0, counts[0], 1,
		      sched, chunk_size);
      gomp_doacross_init (ncounts, counts, chunk_size, extra);
      if (reductions)
	{
	  GOMP_taskgroup_reduction_register (reductions);
	  thr->task->taskgroup->workshare = true;
	  thr->ts.work_share->task_reductions = reductions;
	}
      gomp_work_share_init_done ();
    }
  else
    {
      if (reductions)
	{
	  uintptr_t *first_reductions = thr->ts.work_share->task_reductions;
	  gomp_workshare_task_reduction_register (reductions,
						  first_reductions);
	}
      sched = thr->ts.work_share->sched;
    }

  if (mem)
    *mem = thr->ts.work_share->doacross->extra;

  return ialias_call (GOMP_loop_runtime_next) (istart, iend);
}

/* The *_next routines are called when the thread completes processing of
   the iteration block currently assigned to it.  If the work-share
   construct is bound directly to a parallel construct, then the iteration
   bounds may have been set up before the parallel.  In which case, this
   may be the first iteration for the thread.

   Returns true if there is work remaining to be performed; *ISTART and
   *IEND are filled with a new iteration block.  Returns false if all work
   has been assigned.  */

static bool
gomp_loop_static_next (long *istart, long *iend)
{
  return !gomp_iter_static_next (istart, iend);
}

static bool
gomp_loop_dynamic_next (long *istart, long *iend)
{
  bool ret;

#ifdef HAVE_SYNC_BUILTINS
  ret = gomp_iter_dynamic_next (istart, iend);
#else
  struct gomp_thread *thr = gomp_thread ();
  gomp_mutex_lock (&thr->ts.work_share->lock);
  ret = gomp_iter_dynamic_next_locked (istart, iend);
  gomp_mutex_unlock (&thr->ts.work_share->lock);
#endif

  return ret;
}

static bool
gomp_loop_guided_next (long *istart, long *iend)
{
  bool ret;

#ifdef HAVE_SYNC_BUILTINS
  ret = gomp_iter_guided_next (istart, iend);
#else
  struct gomp_thread *thr = gomp_thread ();
  gomp_mutex_lock (&thr->ts.work_share->lock);
  ret = gomp_iter_guided_next_locked (istart, iend);
  gomp_mutex_unlock (&thr->ts.work_share->lock);
#endif

  return ret;
}

bool
GOMP_loop_runtime_next (long *istart, long *iend)
{
  struct gomp_thread *thr = gomp_thread ();

  switch (thr->ts.work_share->sched)
    {
    case GFS_STATIC:
    case GFS_AUTO:
      return gomp_loop_static_next (istart, iend);
    case GFS_DYNAMIC:
      return gomp_loop_dynamic_next (istart, iend);
    case GFS_GUIDED:
      return gomp_loop_guided_next (istart, iend);
    default:
      abort ();
    }
}

/* The *_ordered_*_next routines are called when the thread completes
   processing of the iteration block currently assigned to it.

   Returns true if there is work remaining to be performed; *ISTART and
   *IEND are filled with a new iteration block.  Returns false if all work
   has been assigned.  */

static bool
gomp_loop_ordered_static_next (long *istart, long *iend)
{
  struct gomp_thread *thr = gomp_thread ();
  int test;

  gomp_ordered_sync ();
  gomp_mutex_lock (&thr->ts.work_share->lock);
  test = gomp_iter_static_next (istart, iend);
  if (test >= 0)
    gomp_ordered_static_next ();
  gomp_mutex_unlock (&thr->ts.work_share->lock);

  return test == 0;
}

static bool
gomp_loop_ordered_dynamic_next (long *istart, long *iend)
{
  struct gomp_thread *thr = gomp_thread ();
  bool ret;

  gomp_ordered_sync ();
  gomp_mutex_lock (&thr->ts.work_share->lock);
  ret = gomp_iter_dynamic_next_locked (istart, iend);
  if (ret)
    gomp_ordered_next ();
  else
    gomp_ordered_last ();
  gomp_mutex_unlock (&thr->ts.work_share->lock);

  return ret;
}

static bool
gomp_loop_ordered_guided_next (long *istart, long *iend)
{
  struct gomp_thread *thr = gomp_thread ();
  bool ret;

  gomp_ordered_sync ();
  gomp_mutex_lock (&thr->ts.work_share->lock);
  ret = gomp_iter_guided_next_locked (istart, iend);
  if (ret)
    gomp_ordered_next ();
  else
    gomp_ordered_last ();
  gomp_mutex_unlock (&thr->ts.work_share->lock);

  return ret;
}

bool
GOMP_loop_ordered_runtime_next (long *istart, long *iend)
{
  struct gomp_thread *thr = gomp_thread ();

  switch (thr->ts.work_share->sched)
    {
    case GFS_STATIC:
    case GFS_AUTO:
      return gomp_loop_ordered_static_next (istart, iend);
    case GFS_DYNAMIC:
      return gomp_loop_ordered_dynamic_next (istart, iend);
    case GFS_GUIDED:
      return gomp_loop_ordered_guided_next (istart, iend);
    default:
      abort ();
    }
}

/* The GOMP_parallel_loop_* routines pre-initialize a work-share construct
   to avoid one synchronization once we get into the loop.  */

static void
gomp_parallel_loop_start (void (*fn) (void *), void *data,
			  unsigned num_threads, long start, long end,
			  long incr, enum gomp_schedule_type sched,
			  long chunk_size, unsigned int flags)
{
  struct gomp_team *team;

  num_threads = gomp_resolve_num_threads (num_threads, 0);
  team = gomp_new_team (num_threads);
  gomp_loop_init (&team->work_shares[0], start, end, incr, sched, chunk_size);
  gomp_team_start (fn, data, num_threads, flags, team, NULL);
}

void
GOMP_parallel_loop_static_start (void (*fn) (void *), void *data,
				 unsigned num_threads, long start, long end,
				 long incr, long chunk_size)
{
  gomp_parallel_loop_start (fn, data, num_threads, start, end, incr,
			    GFS_STATIC, chunk_size, 0);
}

void
GOMP_parallel_loop_dynamic_start (void (*fn) (void *), void *data,
				  unsigned num_threads, long start, long end,
				  long incr, long chunk_size)
{
  gomp_parallel_loop_start (fn, data, num_threads, start, end, incr,
			    GFS_DYNAMIC, chunk_size, 0);
}

void
GOMP_parallel_loop_guided_start (void (*fn) (void *), void *data,
				 unsigned num_threads, long start, long end,
				 long incr, long chunk_size)
{
  gomp_parallel_loop_start (fn, data, num_threads, start, end, incr,
			    GFS_GUIDED, chunk_size, 0);
}

void
GOMP_parallel_loop_runtime_start (void (*fn) (void *), void *data,
				  unsigned num_threads, long start, long end,
				  long incr)
{
  struct gomp_task_icv *icv = gomp_icv (false);
  gomp_parallel_loop_start (fn, data, num_threads, start, end, incr,
			    icv->run_sched_var & ~GFS_MONOTONIC,
			    icv->run_sched_chunk_size, 0);
}

ialias_redirect (GOMP_parallel_end)

void
GOMP_parallel_loop_static (void (*fn) (void *), void *data,
			   unsigned num_threads, long start, long end,
			   long incr, long chunk_size, unsigned flags)
{
  gomp_parallel_loop_start (fn, data, num_threads, start, end, incr,
			    GFS_STATIC, chunk_size, flags);
  fn (data);
  GOMP_parallel_end ();
}

void
GOMP_parallel_loop_dynamic (void (*fn) (void *), void *data,
			    unsigned num_threads, long start, long end,
			    long incr, long chunk_size, unsigned flags)
{
  gomp_parallel_loop_start (fn, data, num_threads, start, end, incr,
			    GFS_DYNAMIC, chunk_size, flags);
  fn (data);
  GOMP_parallel_end ();
}

void
GOMP_parallel_loop_guided (void (*fn) (void *), void *data,
			  unsigned num_threads, long start, long end,
			  long incr, long chunk_size, unsigned flags)
{
  gomp_parallel_loop_start (fn, data, num_threads, start, end, incr,
			    GFS_GUIDED, chunk_size, flags);
  fn (data);
  GOMP_parallel_end ();
}

void
GOMP_parallel_loop_runtime (void (*fn) (void *), void *data,
			    unsigned num_threads, long start, long end,
			    long incr, unsigned flags)
{
  struct gomp_task_icv *icv = gomp_icv (false);
  gomp_parallel_loop_start (fn, data, num_threads, start, end, incr,
			    icv->run_sched_var & ~GFS_MONOTONIC,
			    icv->run_sched_chunk_size, flags);
  fn (data);
  GOMP_parallel_end ();
}

#ifdef HAVE_ATTRIBUTE_ALIAS
extern __typeof(GOMP_parallel_loop_dynamic) GOMP_parallel_loop_nonmonotonic_dynamic
	__attribute__((alias ("GOMP_parallel_loop_dynamic")));
extern __typeof(GOMP_parallel_loop_guided) GOMP_parallel_loop_nonmonotonic_guided
	__attribute__((alias ("GOMP_parallel_loop_guided")));
extern __typeof(GOMP_parallel_loop_runtime) GOMP_parallel_loop_nonmonotonic_runtime
	__attribute__((alias ("GOMP_parallel_loop_runtime")));
extern __typeof(GOMP_parallel_loop_runtime) GOMP_parallel_loop_maybe_nonmonotonic_runtime
	__attribute__((alias ("GOMP_parallel_loop_runtime")));
#else
void
GOMP_parallel_loop_nonmonotonic_dynamic (void (*fn) (void *), void *data,
					 unsigned num_threads, long start,
					 long end, long incr, long chunk_size,
					 unsigned flags)
{
  gomp_parallel_loop_start (fn, data, num_threads, start, end, incr,
			    GFS_DYNAMIC, chunk_size, flags);
  fn (data);
  GOMP_parallel_end ();
}

void
GOMP_parallel_loop_nonmonotonic_guided (void (*fn) (void *), void *data,
					unsigned num_threads, long start,
					long end, long incr, long chunk_size,
					unsigned flags)
{
  gomp_parallel_loop_start (fn, data, num_threads, start, end, incr,
			    GFS_GUIDED, chunk_size, flags);
  fn (data);
  GOMP_parallel_end ();
}

void
GOMP_parallel_loop_nonmonotonic_runtime (void (*fn) (void *), void *data,
					 unsigned num_threads, long start,
					 long end, long incr, unsigned flags)
{
  struct gomp_task_icv *icv = gomp_icv (false);
  gomp_parallel_loop_start (fn, data, num_threads, start, end, incr,
			    icv->run_sched_var & ~GFS_MONOTONIC,
			    icv->run_sched_chunk_size, flags);
  fn (data);
  GOMP_parallel_end ();
}

void
GOMP_parallel_loop_maybe_nonmonotonic_runtime (void (*fn) (void *), void *data,
					       unsigned num_threads, long start,
					       long end, long incr,
					       unsigned flags)
{
  struct gomp_task_icv *icv = gomp_icv (false);
  gomp_parallel_loop_start (fn, data, num_threads, start, end, incr,
			    icv->run_sched_var & ~GFS_MONOTONIC,
			    icv->run_sched_chunk_size, flags);
  fn (data);
  GOMP_parallel_end ();
}
#endif

/* The GOMP_loop_end* routines are called after the thread is told that
   all loop iterations are complete.  The first two versions synchronize
   all threads; the nowait version does not.  */

void
GOMP_loop_end (void)
{
  gomp_work_share_end ();
}

bool
GOMP_loop_end_cancel (void)
{
  return gomp_work_share_end_cancel ();
}

void
GOMP_loop_end_nowait (void)
{
  gomp_work_share_end_nowait ();
}


/* We use static functions above so that we're sure that the "runtime"
   function can defer to the proper routine without interposition.  We
   export the static function with a strong alias when possible, or with
   a wrapper function otherwise.  */

#ifdef HAVE_ATTRIBUTE_ALIAS
extern __typeof(gomp_loop_static_start) GOMP_loop_static_start
	__attribute__((alias ("gomp_loop_static_start")));
extern __typeof(gomp_loop_dynamic_start) GOMP_loop_dynamic_start
	__attribute__((alias ("gomp_loop_dynamic_start")));
extern __typeof(gomp_loop_guided_start) GOMP_loop_guided_start
	__attribute__((alias ("gomp_loop_guided_start")));
extern __typeof(gomp_loop_dynamic_start) GOMP_loop_nonmonotonic_dynamic_start
	__attribute__((alias ("gomp_loop_dynamic_start")));
extern __typeof(gomp_loop_guided_start) GOMP_loop_nonmonotonic_guided_start
	__attribute__((alias ("gomp_loop_guided_start")));
extern __typeof(GOMP_loop_runtime_start) GOMP_loop_nonmonotonic_runtime_start
	__attribute__((alias ("GOMP_loop_runtime_start")));
extern __typeof(GOMP_loop_runtime_start) GOMP_loop_maybe_nonmonotonic_runtime_start
	__attribute__((alias ("GOMP_loop_runtime_start")));

extern __typeof(gomp_loop_ordered_static_start) GOMP_loop_ordered_static_start
	__attribute__((alias ("gomp_loop_ordered_static_start")));
extern __typeof(gomp_loop_ordered_dynamic_start) GOMP_loop_ordered_dynamic_start
	__attribute__((alias ("gomp_loop_ordered_dynamic_start")));
extern __typeof(gomp_loop_ordered_guided_start) GOMP_loop_ordered_guided_start
	__attribute__((alias ("gomp_loop_ordered_guided_start")));

extern __typeof(gomp_loop_doacross_static_start) GOMP_loop_doacross_static_start
	__attribute__((alias ("gomp_loop_doacross_static_start")));
extern __typeof(gomp_loop_doacross_dynamic_start) GOMP_loop_doacross_dynamic_start
	__attribute__((alias ("gomp_loop_doacross_dynamic_start")));
extern __typeof(gomp_loop_doacross_guided_start) GOMP_loop_doacross_guided_start
	__attribute__((alias ("gomp_loop_doacross_guided_start")));

extern __typeof(gomp_loop_static_next) GOMP_loop_static_next
	__attribute__((alias ("gomp_loop_static_next")));
extern __typeof(gomp_loop_dynamic_next) GOMP_loop_dynamic_next
	__attribute__((alias ("gomp_loop_dynamic_next")));
extern __typeof(gomp_loop_guided_next) GOMP_loop_guided_next
	__attribute__((alias ("gomp_loop_guided_next")));
extern __typeof(gomp_loop_dynamic_next) GOMP_loop_nonmonotonic_dynamic_next
	__attribute__((alias ("gomp_loop_dynamic_next")));
extern __typeof(gomp_loop_guided_next) GOMP_loop_nonmonotonic_guided_next
	__attribute__((alias ("gomp_loop_guided_next")));
extern __typeof(GOMP_loop_runtime_next) GOMP_loop_nonmonotonic_runtime_next
	__attribute__((alias ("GOMP_loop_runtime_next")));
extern __typeof(GOMP_loop_runtime_next) GOMP_loop_maybe_nonmonotonic_runtime_next
	__attribute__((alias ("GOMP_loop_runtime_next")));

extern __typeof(gomp_loop_ordered_static_next) GOMP_loop_ordered_static_next
	__attribute__((alias ("gomp_loop_ordered_static_next")));
extern __typeof(gomp_loop_ordered_dynamic_next) GOMP_loop_ordered_dynamic_next
	__attribute__((alias ("gomp_loop_ordered_dynamic_next")));
extern __typeof(gomp_loop_ordered_guided_next) GOMP_loop_ordered_guided_next
	__attribute__((alias ("gomp_loop_ordered_guided_next")));
#else
bool
GOMP_loop_static_start (long start, long end, long incr, long chunk_size,
			long *istart, long *iend)
{
  return gomp_loop_static_start (start, end, incr, chunk_size, istart, iend);
}

bool
GOMP_loop_dynamic_start (long start, long end, long incr, long chunk_size,
			 long *istart, long *iend)
{
  return gomp_loop_dynamic_start (start, end, incr, chunk_size, istart, iend);
}

bool
GOMP_loop_guided_start (long start, long end, long incr, long chunk_size,
			long *istart, long *iend)
{
  return gomp_loop_guided_start (start, end, incr, chunk_size, istart, iend);
}

bool
GOMP_loop_nonmonotonic_dynamic_start (long start, long end, long incr,
				      long chunk_size, long *istart,
				      long *iend)
{
  return gomp_loop_dynamic_start (start, end, incr, chunk_size, istart, iend);
}

bool
GOMP_loop_nonmonotonic_guided_start (long start, long end, long incr,
				     long chunk_size, long *istart, long *iend)
{
  return gomp_loop_guided_start (start, end, incr, chunk_size, istart, iend);
}

bool
GOMP_loop_nonmonotonic_runtime_start (long start, long end, long incr,
				      long *istart, long *iend)
{
  return GOMP_loop_runtime_start (start, end, incr, istart, iend);
}

bool
GOMP_loop_maybe_nonmonotonic_runtime_start (long start, long end, long incr,
					    long *istart, long *iend)
{
  return GOMP_loop_runtime_start (start, end, incr, istart, iend);
}

bool
GOMP_loop_ordered_static_start (long start, long end, long incr,
				long chunk_size, long *istart, long *iend)
{
  return gomp_loop_ordered_static_start (start, end, incr, chunk_size,
					 istart, iend);
}

bool
GOMP_loop_ordered_dynamic_start (long start, long end, long incr,
				 long chunk_size, long *istart, long *iend)
{
  return gomp_loop_ordered_dynamic_start (start, end, incr, chunk_size,
					  istart, iend);
}

bool
GOMP_loop_ordered_guided_start (long start, long end, long incr,
				long chunk_size, long *istart, long *iend)
{
  return gomp_loop_ordered_guided_start (start, end, incr, chunk_size,
					 istart, iend);
}

bool
GOMP_loop_doacross_static_start (unsigned ncounts, long *counts,
				 long chunk_size, long *istart, long *iend)
{
  return gomp_loop_doacross_static_start (ncounts, counts, chunk_size,
					  istart, iend);
}

bool
GOMP_loop_doacross_dynamic_start (unsigned ncounts, long *counts,
				  long chunk_size, long *istart, long *iend)
{
  return gomp_loop_doacross_dynamic_start (ncounts, counts, chunk_size,
					   istart, iend);
}

bool
GOMP_loop_doacross_guided_start (unsigned ncounts, long *counts,
				 long chunk_size, long *istart, long *iend)
{
  return gomp_loop_doacross_guided_start (ncounts, counts, chunk_size,
					  istart, iend);
}

bool
GOMP_loop_static_next (long *istart, long *iend)
{
  return gomp_loop_static_next (istart, iend);
}

bool
GOMP_loop_dynamic_next (long *istart, long *iend)
{
  return gomp_loop_dynamic_next (istart, iend);
}

bool
GOMP_loop_guided_next (long *istart, long *iend)
{
  return gomp_loop_guided_next (istart, iend);
}

bool
GOMP_loop_nonmonotonic_dynamic_next (long *istart, long *iend)
{
  return gomp_loop_dynamic_next (istart, iend);
}

bool
GOMP_loop_nonmonotonic_guided_next (long *istart, long *iend)
{
  return gomp_loop_guided_next (istart, iend);
}

bool
GOMP_loop_nonmonotonic_runtime_next (long *istart, long *iend)
{
  return GOMP_loop_runtime_next (istart, iend);
}

bool
GOMP_loop_maybe_nonmonotonic_runtime_next (long *istart, long *iend)
{
  return GOMP_loop_runtime_next (istart, iend);
}

bool
GOMP_loop_ordered_static_next (long *istart, long *iend)
{
  return gomp_loop_ordered_static_next (istart, iend);
}

bool
GOMP_loop_ordered_dynamic_next (long *istart, long *iend)
{
  return gomp_loop_ordered_dynamic_next (istart, iend);
}

bool
GOMP_loop_ordered_guided_next (long *istart, long *iend)
{
  return gomp_loop_ordered_guided_next (istart, iend);
}
#endif
