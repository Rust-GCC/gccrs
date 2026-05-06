/* A simple allocation cache.
   Copyright (C) 2026 Free Software Foundation, Inc.

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

#ifndef _ALLOC_CACHE_H
#define _ALLOC_CACHE_H

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <pthread.h>

/* A single cached allocation.  All fields immutable.  */
struct alloc_cache_node
{
  /* When taken, someone is using this node, and we can't.  */
  pthread_mutex_t lock;
  void *allocation;
  size_t size;

  struct alloc_cache_node *next;
};

struct alloc_cache
{
  _Atomic (struct alloc_cache_node *) head;

  /* Could be made better by breaking it up into buckets eventually.  Our
     current allocation pattern is such that most accesses are likely to only
     ever use the smallest practical allocation, so there isn't much gain in
     implementing buckets currently.

     Currently, as it is used, this cache will likely be of size O(T) where T
     is the max number of concurrently executing kernels during the lifetime of
     the process.  I suspect this value is low, so even with a single bucket,
     it is likely fast enough to search through.  */
};

/* Prepare CACHE for use, initializing it as empty.  */
static inline void
init_alloc_cache (struct alloc_cache *cache)
{
  atomic_init (&cache->head, NULL);
}

/* Search through CACHE, looking for a non-taken node of large enough to fit
   DESIRED_SIZE bytes.  Returns NULL if no such node exists.  */
static inline struct alloc_cache_node *
alloc_cache_try_find (struct alloc_cache *cache, size_t desired_size)
{
  for (struct alloc_cache_node *node =
	 atomic_load_explicit (&cache->head, memory_order_relaxed);
       node;
       node = node->next)
    {
      if (node->size < desired_size)
	continue;

      int ret;
      if ((ret = pthread_mutex_trylock (&node->lock)) == EBUSY)
	continue;
      assert (ret == 0);

      /* It worked!  We found a node that's large enough and free.  */
      return node;
    }

  return NULL;
}

/* Add a new node for allocation ALLOCATION of SIZE bytes into the cache.  The
   new node is acquired on return.  */
static inline struct alloc_cache_node *
alloc_cache_add_taken_node (struct alloc_cache *cache,
			    void *allocation,
			    size_t size)
{
  struct alloc_cache_node *new_node = malloc (sizeof (*new_node));

  if (!new_node)
    return NULL;

  *new_node = (struct alloc_cache_node) {
    .lock = PTHREAD_MUTEX_INITIALIZER,
    .allocation = allocation,
    .size = size,
    .next = NULL
  };
  pthread_mutex_lock (&new_node->lock);

  /* Place it on the top of the stack.  */
  struct alloc_cache_node *top = (atomic_load_explicit
				  (&cache->head, memory_order_acquire));

  do new_node->next = top;
  while (!atomic_compare_exchange_weak_explicit
	 (&cache->head, &top, new_node,
	  memory_order_acq_rel, memory_order_acquire));

  return new_node;
}

/* Allow NODE to be used by other users of its cache.  */
static inline void
release_alloc_cache_node (struct alloc_cache_node *node)
{
  pthread_mutex_unlock (&node->lock);
}

/* Destroy NODE.  Caller is responsible for cleaning up the allocation inside
   of NODE, and for making sure that it is not part of any cache that is going
   to be used in the future.  */
static inline void
destroy_alloc_cache_node (struct alloc_cache_node *node)
{
  pthread_mutex_destroy (&node->lock);
  free (node);
}


#endif /* _ALLOC_CACHE_H  */
