/* { dg-do compile } */
#include <omp.h>

void
f ()
{
   omp_alloctrait_t trait[1] = {{omp_atk_sync_hint, 1}};
   omp_allocator_handle_t my_alloc;
   #pragma omp target uses_allocators(traits(trait) : my_alloc)  /* { dg-error "traits array 'trait' must be of 'const omp_alloctrait_t \\\[\\\]' type" } */
     ;
}

void
g ()
{
   const omp_alloctrait_t trait[1] = {{omp_atk_sync_hint, 1}};
   omp_allocator_handle_t my_alloc;
   #pragma omp target uses_allocators(traits(trait) : my_alloc)
     ;
}
