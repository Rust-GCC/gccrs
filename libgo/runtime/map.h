/* map.h -- the map type for Go.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <stddef.h>

#include "go-type.h"

/* A map descriptor is what we need to manipulate the map.  This is
   constant for a given map type.  */

struct __go_map_descriptor
{
  /* A pointer to the type descriptor for the key type.  */
  const struct __go_map_type *__map_descriptor;

  /* A map entry is a struct with three fields:
       map_entry_type *next_entry;
       key_type key;
       value_type value;
     This is the size of that struct.  */
  size_t __entry_size;

  /* The offset of the key field in a map entry struct.  */
  size_t __key_offset;

  /* The offset of the value field in a map entry struct (the value
     field immediately follows the key field, but there may be some
     bytes inserted for alignment).  */
  size_t __val_offset;
};

struct __go_map
{
  /* The constant descriptor for this map.  */
  const struct __go_map_descriptor *__descriptor;

  /* The number of elements in the hash table.  */
  size_t __element_count;

  /* The number of entries in the __buckets array.  */
  size_t __bucket_count;

  /* Each bucket is a pointer to a linked list of map entries.  */
  void **__buckets;
};

extern struct __go_map *__go_new_map (const struct __go_map_descriptor *,
				      size_t);

extern unsigned long __go_map_next_prime (unsigned long);

extern void *__go_map_index (struct __go_map *, const void *, _Bool);

extern void __go_map_delete (struct __go_map *, const void *);

extern _Bool __go_map_range (const struct __go_map *, size_t *,
			     const void **, const void **, const void **);
