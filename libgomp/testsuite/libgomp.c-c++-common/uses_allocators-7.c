/* { dg-do compile } */
#include <omp.h>

void f()
{
 omp_allocator_handle_t my;
 struct t {
   omp_allocator_handle_t a1;
 } s;

const omp_allocator_handle_t my3 = (omp_allocator_handle_t) 300;
omp_allocator_handle_t my4[1];
const omp_alloctrait_t t[] = {};
 #pragma omp target uses_allocators(my, omp_default_mem_alloc, omp_null_allocator)  // OK
   ;
 #pragma omp target uses_allocators(my) firstprivate(my) // { dg-error "'my' appears more than once in data clauses" }
   ;
 #pragma omp target private(my) uses_allocators(my) // { dg-error "'my' appears more than once in data clauses" }
   ;
 #pragma omp target uses_allocators(my3)
   ;
 #pragma omp target uses_allocators(s.a1)
   // { dg-error "expected '\\)' before '\\.' token" "" { target *-*-* } .-1 }
   // { dg-error "allocator 's' must be of 'omp_allocator_handle_t' type" "" { target *-*-* } .-2 }
   ;
 #pragma omp target uses_allocators(my4)
   // { dg-error "allocator 'my4' must be of 'omp_allocator_handle_t' type" "" { target *-*-* } .-1 }
   ;
 #pragma omp target uses_allocators(my4[0])
   // { dg-error "expected '\\)' before '\\\[' token" "" { target *-*-* } .-1 }
   // { dg-error "allocator 'my4' must be of 'omp_allocator_handle_t' type" "" { target *-*-* } .-2 }
   ;
 #pragma omp target uses_allocators(memspace(omp_default_mem_space) : my, my(t)) // { dg-error "legacy 'my\\(t\\)' traits syntax not allowed in 'uses_allocators' clause when using modifiers" }
   ;
}
