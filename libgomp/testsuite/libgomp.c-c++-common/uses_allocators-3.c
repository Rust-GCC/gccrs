/* { dg-do compile } */
/* { dg-additional-options "-fdump-tree-original -fdump-tree-gimple" } */

#include <omp.h>

int n = 2;

int main (void)
{
  omp_allocator_handle_t foo, bar;
  const omp_alloctrait_t foo_traits[] = { { omp_atk_pinned,    omp_atv_true },
  					  { omp_atk_partition, omp_atv_nearest } };

  #pragma omp target
    ;
  #pragma omp target uses_allocators (bar)
    ;
  #pragma omp target uses_allocators (foo (foo_traits)) /* { dg-warning "the specification of arguments to 'uses_allocators' where each item is of the form 'allocator\\(traits\\)' is deprecated since OpenMP 5.2 \\\[-Wdeprecated-openmp\\\]" } */
    ;
  #pragma omp target uses_allocators (foo (foo_traits), bar (foo_traits)) /* { dg-warning "the specification of arguments to 'uses_allocators' where each item is of the form 'allocator\\(traits\\)' is deprecated since OpenMP 5.2 \\\[-Wdeprecated-openmp\\\]" } */
    ;
  #pragma omp target uses_allocators (memspace(omp_high_bw_mem_space) : foo)
    ;
  #pragma omp target uses_allocators (traits(foo_traits) : bar)
    ;
  #pragma omp target parallel uses_allocators (memspace(omp_high_bw_mem_space), traits(foo_traits) : bar)
    ;
  #pragma omp target parallel uses_allocators (traits(foo_traits), memspace(omp_high_bw_mem_space) : bar) uses_allocators(foo)
  {
    void *p = omp_alloc ((unsigned long) 32, bar);
    omp_free (p, bar);
  }
  return 0;
}

/* { dg-final { scan-tree-dump "pragma omp target uses_allocators\\(memspace\\(\\), traits\\(\\) : bar\\)" "original" } } */
/* { dg-final { scan-tree-dump "pragma omp target uses_allocators\\(memspace\\(\\), traits\\(foo_traits\\) : foo\\)" "original" } } */
/* { dg-final { scan-tree-dump "pragma omp target uses_allocators\\(memspace\\(\\), traits\\(foo_traits\\) : bar\\) uses_allocators\\(memspace\\(\\), traits\\(foo_traits\\) : foo\\)" "original" } } */
/* { dg-final { scan-tree-dump "pragma omp target uses_allocators\\(memspace\\(.+\\), traits\\(\\) : foo\\)" "original" } } */
/* { dg-final { scan-tree-dump "pragma omp target uses_allocators\\(memspace\\(\\), traits\\(foo_traits\\) : bar\\)" "original" } } */
/* { dg-final { scan-tree-dump "pragma omp target uses_allocators\\(memspace\\(.+\\), traits\\(foo_traits\\) : bar\\)" "original" } } */
/* { dg-final { scan-tree-dump "pragma omp target uses_allocators\\(memspace\\(.+\\), traits\\(foo_traits\\) : bar\\) uses_allocators\\(memspace\\(\\), traits\\(\\) : foo\\)" "original" } } */

/* { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) uses_allocators\\(memspace\\(\\), traits\\(\\) : bar\\)" "gimple" } } */
/* { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) uses_allocators\\(memspace\\(\\), traits\\(foo_traits\\) : foo\\)" "gimple" } } */
/* { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) uses_allocators\\(memspace\\(\\), traits\\(foo_traits\\) : bar\\) uses_allocators\\(memspace\\(\\), traits\\(foo_traits\\) : foo\\)" "gimple" } } */
/* { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) uses_allocators\\(memspace\\(.+\\), traits\\(\\) : foo\\)" "gimple" } } */
/* { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) uses_allocators\\(memspace\\(\\), traits\\(foo_traits\\) : bar\\)" "gimple" } } */
/* { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) uses_allocators\\(memspace\\(.+\\), traits\\(foo_traits\\) : bar\\)" "gimple" } } */
/* { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) uses_allocators\\(memspace\\(.+\\), traits\\(foo_traits\\) : bar\\) uses_allocators\\(memspace\\(\\), traits\\(\\) : foo\\)" "gimple" } } */
