// { dg-do compile }
// { dg-additional-options "-fdump-tree-original" }

#include <omp.h>

void f()
{
 omp_allocator_handle_t my, my2, my3, my4;
const omp_alloctrait_t t[] = {};
const omp_alloctrait_t t2[] = {};
 #pragma omp target uses_allocators(traits(t), memspace(omp_high_bw_mem_space) : my; omp_default_mem_alloc, omp_null_allocator; my2; traits(t2) : my3; memspace(omp_large_cap_mem_space) : my4)
   ;
}

// { dg-final { scan-tree-dump "#pragma omp target uses_allocators\\(memspace\\(1\\), traits\\(\\) : my4\\) uses_allocators\\(memspace\\(\\), traits\\(t2\\) : my3\\) uses_allocators\\(memspace\\(\\), traits\\(\\) : my2\\) uses_allocators\\(memspace\\(3\\), traits\\(t\\) : my\\)" "original" { target c } } }
// { dg-final { scan-tree-dump "#pragma omp target uses_allocators\\(memspace\\(omp_large_cap_mem_space\\), traits\\(\\) : my4\\) uses_allocators\\(memspace\\(\\), traits\\(t2\\) : my3\\) uses_allocators\\(memspace\\(\\), traits\\(\\) : my2\\) uses_allocators\\(memspace\\(omp_high_bw_mem_space\\), traits\\(t\\) : my\\)" "original" { target c++ } } }
