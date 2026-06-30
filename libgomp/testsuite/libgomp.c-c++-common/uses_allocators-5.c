/* { dg-do compile } */
/* { dg-additional-options "-fdump-tree-original -fdump-tree-gimple" } */

#include <omp.h>

int main (void)
{
  omp_allocator_handle_t memspace, traits;
  const omp_alloctrait_t mytraits[] = { { omp_atk_pinned,    omp_atv_true },
					{ omp_atk_partition, omp_atv_nearest } };
  #pragma omp target uses_allocators (memspace)
    ;
  #pragma omp target uses_allocators (traits)
    ;
  #pragma omp target uses_allocators (traits, memspace)
    ;
  #pragma omp target uses_allocators (traits (mytraits)) /* { dg-warning "the specification of arguments to 'uses_allocators' where each item is of the form 'allocator\\(traits\\)' is deprecated since OpenMP 5.2" } */
    ;
  #pragma omp target uses_allocators (memspace (mytraits), omp_default_mem_alloc) /* { dg-warning "the specification of arguments to 'uses_allocators' where each item is of the form 'allocator\\(traits\\)' is deprecated since OpenMP 5.2" } */
    ;
  return 0;
}

/* { dg-final { scan-tree-dump "pragma omp target uses_allocators\\(memspace\\(\\), traits\\(\\) : memspace\\)" "original" } } */
/* { dg-final { scan-tree-dump "pragma omp target uses_allocators\\(memspace\\(\\), traits\\(\\) : traits\\)" "original" } } */
/* { dg-final { scan-tree-dump "pragma omp target uses_allocators\\(memspace\\(\\), traits\\(\\) : memspace\\) uses_allocators\\(memspace\\(\\), traits\\(\\) : traits\\)" "original" } } */
/* { dg-final { scan-tree-dump "pragma omp target uses_allocators\\(memspace\\(\\), traits\\(mytraits\\) : traits\\)" "original" } } */
/* { dg-final { scan-tree-dump "pragma omp target uses_allocators\\(memspace\\(\\), traits\\(mytraits\\) : memspace\\)" "original" } } */

/* { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) uses_allocators\\(memspace\\(\\), traits\\(\\) : memspace\\)" "gimple" } } */
/* { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) uses_allocators\\(memspace\\(\\), traits\\(\\) : traits\\)" "gimple" } } */
/* { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) uses_allocators\\(memspace\\(\\), traits\\(\\) : memspace\\) uses_allocators\\(memspace\\(\\), traits\\(\\) : traits\\)" "gimple" } } */
/* { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) uses_allocators\\(memspace\\(\\), traits\\(mytraits\\) : traits\\)" "gimple" } } */
/* { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) uses_allocators\\(memspace\\(\\), traits\\(mytraits\\) : memspace\\)" "gimple" } } */
