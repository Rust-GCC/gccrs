/* go-map-range.c -- implement a range clause over a map.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include "map.h"

/* A map iteration uses two variables: an index into the buckets and a
   pointer to a map entry.  The index is initially zero and the
   pointer is initially NULL.  This function updates them to point to
   the next entry we care about.  It sets *PKEY and *PVALUE to point
   to the key and value of that entry.  It returns true if there is a
   new entry, false at the end of the iteration.  */

_Bool
__go_map_range (const struct __go_map *map, size_t *pbucket,
		const void **pentry,
		const void **pkey, const void **pvalue)
{
  size_t bucket;
  const char *entry;

  if (map == NULL)
    return 0;

  /* Since *PBUCKET starts at zero, we use that to signal
     initialization, and always subtract one for the actual bucket
     number.  */

  bucket = *pbucket;
  if (bucket == 0)
    {
      /* This is the first call to this function in this
	 iteration.  */
      entry = NULL;
    }
  else
    {
      /* In the previous iteration, we set *PENTRY to point to the
	 next entry in the current bucket.  That is, it is the entry
	 that we should return now.  */
      entry = (const char *) *pentry;
    }

  if (entry == NULL)
    {
      while (1)
	{
	  ++bucket;
	  if (bucket - 1 >= map->__bucket_count)
	    return 0;
	  entry = (const char *) map->__buckets[bucket - 1];
	  if (entry != NULL)
	    break;
	}
    }

  /* The first field of ENTRY is a pointer to the next entry in this
     bucket.  Set *PENTRY to point to that next entry, the one which
     we should return next time.  Tracking the next entry permits the
     current entry to be deleted.  */
  *pentry = *(const void * const *) entry;

  *pbucket = bucket;
  *pkey = entry + map->__descriptor->__key_offset;
  if (pvalue != NULL)
    *pvalue = entry + map->__descriptor->__val_offset;
  return 1;
}
