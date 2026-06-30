/* { dg-do compile } */
/* { dg-additional-options "-Wno-deprecated-openmp" } */

#include <omp.h>

int f (const omp_alloctrait_t arg_traits[], int n)
{
  omp_allocator_handle_t foo, bar;
  const omp_alloctrait_t traits_array[] = { { omp_atk_pinned,    omp_atv_true },
					    { omp_atk_partition, omp_atv_nearest } };
  extern const omp_alloctrait_t ex_traits[2];
  extern const omp_alloctrait_t ex2_traits[];
#ifndef __cplusplus
  const omp_alloctrait_t vla_traits[n] = {};  /* Not useful, but shouldn't crash.  */
#else
  const omp_alloctrait_t vla_traits[n] = { { omp_atk_pinned,    omp_atv_true },
  					   { omp_atk_partition, omp_atv_nearest } };
#endif

  #pragma omp target uses_allocators (baz) /* { dg-error "'baz' undeclared .first use in this function." "" { target c } } */
    ;                                      /* { dg-error "'baz' has not been declared" "" { target c++ } .-1 } */
  #pragma omp target uses_allocators (foo (xyz)) /* { dg-error "'xyz' undeclared .first use in this function." "" { target c } } */
    ;                                            /* { dg-error "'xyz' has not been declared" "" { target c++ } .-1 } */
  #pragma omp target uses_allocators (foo (traits_array), baz (traits_array)) /* { dg-error "'baz' has not been declared" "" { target c++ } } */
    ;
  #pragma omp target uses_allocators (foo (arg_traits)) /* { dg-error "traits array 'arg_traits' must be defined in same scope as the construct on which the clause appears" } */
    ;                                                   /* { dg-error "traits array 'arg_traits' must be of 'const omp_alloctrait_t \\\[\\\]' type" "" { target *-*-* } .-1 } */
  #pragma omp target uses_allocators (foo (ex_traits)) /* { dg-error "traits array 'ex_traits' must be defined in same scope as the construct on which the clause appears" } */
    ;                                                  /* { dg-error "traits array must be initialized with constants" "" { target *-*-* } .-1 } */
  #pragma omp target uses_allocators (foo (ex2_traits)) /* { dg-error "traits array 'ex2_traits' must be defined in same scope as the construct on which the clause appears" } */
    ;                                                   /* { dg-error "traits array 'ex2_traits' must be of 'const omp_alloctrait_t \\\[\\\]' type" "" { target *-*-* } .-1 } */
  #pragma omp target uses_allocators (foo (vla_traits)) /* { dg-error "variable length traits arrays are not supported" "" { target c++ } } */
    ;
  #pragma omp target uses_allocators (memspace(omp_no_such_space) : foo) /* { dg-error "'omp_no_such_space' undeclared .first use in this function." "" { target c } } */
    ;                                                                    /* { dg-error "'omp_no_such_space' was not declared in this scope" "" { target c++ } .-1 } */
  #pragma omp target uses_allocators (memspace(1) : foo) /* { dg-error "memspace modifier '1' must be constant enum of 'omp_memspace_handle_t' type" } */
    ;
  #pragma omp target uses_allocators (memspace(omp_no_such_space) : foo, bar) /* { dg-error "'uses_allocators' clause only accepts a single allocator when using modifiers" } */
    ;                                                                         /* { dg-error "memspace modifier 'omp_no_such_space' must be constant enum of 'omp_memspace_handle_t' type" "" { target c++ } .-1 } */
  #pragma omp target uses_allocators (traits(xyz) : bar) /* { dg-error "'xyz' was not declared in this scope" "" { target c++ } } */
    ;
  #pragma omp target uses_allocators (memspace(omp_high_bw_mem_space), traits(traits_array), memspace (omp_no_such_space) : bar) /* { dg-error "duplicate 'memspace' modifier" "" { target c } } */
    ;                                                                                                                            /* { dg-error "expected '\\\)' before 'memspace" "" { target c } .-1 } */
                                                                                                                                 /* { dg-error "duplicate 'memspace' modifier" "" { target c++ } .-2 } */
  #pragma omp target uses_allocators (traitz(traits_array), memspace(omp_high_bw_mem_space) : bar) /* { dg-error "'traitz' undeclared .first use in this function." "" { target c } } */
    ;                                                                                              /* { dg-error "'memspace' undeclared .first use in this function." "" { target c } .-1 } */
                                                                                                   /* { dg-error "'traitz' has not been declared" "" { target c++ } .-2 } */
                                                                                                   /* { dg-error "'memspace' has not been declared" "" { target c++ } .-3 } */
                                                                                                   /* { dg-error "expected '\\\)' before ':' token" "" { target *-*-* } .-4 } */
  #pragma omp target uses_allocators (omp_null_allocator)
    ;
  #pragma omp target uses_allocators (memspace(omp_high_bw_mem_space) : foo, bar) /* { dg-error "'uses_allocators' clause only accepts a single allocator when using modifiers" } */
    ;
  #pragma omp target uses_allocators (memspace(omp_high_bw_mem_space) : foo(foo_traits)) /* { dg-error "'foo_traits' undeclared .first use in this function.; did you mean 'vla_traits'." "" { target c } } */
    ;                                                                                    /* { dg-error "'foo_traits' has not been declared" "" { target c++ } .-1 } */
                                                                                         /* { dg-error "legacy 'foo\\\(foo_traits\\\)' traits syntax not allowed in 'uses_allocators' clause when using modifiers" "" { target *-*-* } .-2 } */
  return 0;
}
