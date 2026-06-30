! { dg-do compile }
! { dg-additional-options "-fdump-tree-original -fdump-tree-gimple" }

program main
  use omp_lib
  implicit none
  integer, allocatable :: arr(:)
  integer (omp_allocator_handle_kind) :: bar, foo

  type (omp_alloctrait), parameter :: traits_array(*) = &
       [omp_alloctrait(omp_atk_pinned,omp_atv_true),&
       omp_alloctrait(omp_atk_partition,omp_atv_nearest)]

  !$omp target allocate(bar : arr) uses_allocators(bar)
  block
    allocate(arr(100))
  end block

  !$omp target uses_allocators(omp_default_mem_alloc)
  block
  end block

  !$omp target uses_allocators(bar(traits_array), foo (traits_array)) ! { dg-warning "The specification of arguments to 'uses_allocators' at \\(1\\) where each item is of the form 'allocator\\(traits\\)' is deprecated since OpenMP 5.2; instead use 'uses_allocators\\(traits\\(traits_array\\): foo\\)' \\\[-Wdeprecated-openmp\\\]" }
  block
    if (foo == 0) stop 1
  end block

  !$omp target uses_allocators(traits(traits_array) : bar)
  block
  end block

  !$omp target parallel uses_allocators(memspace (omp_low_lat_mem_space) : bar)
  block
  end block

  !$omp target parallel uses_allocators(memspace (omp_high_bw_mem_space), traits(traits_array) : bar)
  block
    use iso_c_binding
    type(c_ptr) :: ptr
    integer(c_size_t) :: sz = 32
    ptr = omp_alloc (sz, bar)
    call omp_free (ptr, bar)
  end block

end program main

! { dg-final { scan-tree-dump "pragma omp target allocate\\(allocator\\(bar\\):arr\\) uses_allocators\\(memspace\\(\\), traits\\(\\) : bar\\)" "original" } }
! { dg-final { scan-tree-dump "pragma omp target" "original" } }
! { dg-final { scan-tree-dump "pragma omp target uses_allocators\\(memspace\\(\\), traits\\(traits_array\\) : bar\\) uses_allocators\\(memspace\\(\\), traits\\(traits_array\\) : foo\\)" "original" } }
! { dg-final { scan-tree-dump "pragma omp target uses_allocators\\(memspace\\(\\), traits\\(traits_array\\) : bar\\)" "original" } }
! { dg-final { scan-tree-dump "pragma omp target uses_allocators\\(memspace\\(omp_low_lat_mem_space\\), traits\\(\\) : bar\\)" "original" } }
! { dg-final { scan-tree-dump "pragma omp target uses_allocators\\(memspace\\(omp_high_bw_mem_space\\), traits\\(traits_array\\) : bar\\)" "original" } }

! { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) allocate\\(allocator\\(bar\\):arr\\) uses_allocators\\(memspace\\(\\), traits\\(\\) : bar\\)" "gimple" } }
! { dg-final { scan-tree-dump "pragma omp target" "gimple" } }
! { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) uses_allocators\\(memspace\\(\\), traits\\(traits_array\\) : bar\\) uses_allocators\\(memspace\\(\\), traits\\(traits_array\\) : foo\\)" "gimple" } }
! { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) uses_allocators\\(memspace\\(\\), traits\\(traits_array\\) : bar\\)" "gimple" } }
! { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) uses_allocators\\(memspace\\(omp_low_lat_mem_space\\), traits\\(\\) : bar\\)" "gimple" } }
! { dg-final { scan-tree-dump "pragma omp target num_teams\\(-2\\) thread_limit\\(0\\) uses_allocators\\(memspace\\(omp_high_bw_mem_space\\), traits\\(traits_array\\) : bar\\)" "gimple" } }
