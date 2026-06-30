! { dg-do compile }
use iso_c_binding
use omp_lib
implicit none
contains
subroutine x
integer :: mem
type(omp_alloctrait), parameter:: mem2(1) = [omp_alloctrait(1,1)]
integer(omp_allocator_handle_kind) :: var
!$omp target uses_allocators(memspace(omp_default_mem_space), traits(mem2) : var) defaultmap(none)
block;
type(c_ptr) ::c
c = omp_alloc(omp_default_mem_space, 20_8)
end block
!$omp target uses_allocators(omp_default_mem_alloc, var(mem2))  ! { dg-warning "The specification of arguments to 'uses_allocators' at \\(1\\) where each item is of the form 'allocator\\(traits\\)' is deprecated since OpenMP 5.2; instead use 'uses_allocators\\(traits\\(mem2\\): var\\)' \\\[-Wdeprecated-openmp\\\]" }
block; end block
end
end
