/* { dg-do run } */
/* Unit-test the alloc cache DS.  */
#include <assert.h>
#include <stdint.h>

#include "alloc_cache.h"

int
main ()
{
  struct alloc_cache cache;

  init_alloc_cache (&cache);

  /* Empty cache.  Should return NULL.  */
  assert (alloc_cache_try_find (&cache, 16) == NULL);
  assert (alloc_cache_try_find (&cache, 0) == NULL);

  /* Populating it a bit.  */
  {
    for (int i = 0; i < 5; i++)
      {
	uintptr_t x = 1 << i;
	__auto_type n = alloc_cache_add_taken_node (&cache, (void *) x, x);
	assert (n);
	assert (n->allocation == (void *)x);
	release_alloc_cache_node (n);
      }
  }

  /* Taking five things, each of size 1, should return the whole cache.  */
  {
    struct alloc_cache_node *n[5];
    uint32_t gotten_nodes = 0;
    for (int i = 0; i < 5; i++)
      {
	__auto_type node = n[i] = alloc_cache_try_find (&cache, 1);
	uintptr_t x = (uintptr_t) node->allocation;
	gotten_nodes |= x;
	assert (x == 1
		|| x == 2
		|| x == 4
		|| x == 8
		|| x == 16);
      }
    assert (gotten_nodes == 0b11111);

    /* ... and the cache should remain empty.  */
    assert (alloc_cache_try_find (&cache, 0) == NULL);

    for (int i = 0; i < 5; i++)
      release_alloc_cache_node (n[i]);
  }

  /* Taking 16 twice should fail the second time.  */
  {
    __auto_type n = alloc_cache_try_find (&cache, 16);
    assert (n != NULL && ((uintptr_t) n->allocation) == 16);
    assert (alloc_cache_try_find (&cache, 16) == NULL);
    release_alloc_cache_node (n);
  }
}
