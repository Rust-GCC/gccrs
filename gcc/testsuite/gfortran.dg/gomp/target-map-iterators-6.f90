! { dg-do compile }
! { dg-options "-fopenmp -fdump-tree-omplower" }

module m
  implicit none
  integer, parameter :: DIM = 17
  type :: array_ptr
    integer, pointer :: ptr(:)
  end type
contains
  subroutine f(x)
    type (array_ptr) :: x(DIM)
    integer :: i

    !$omp target data map(iterator(it = 1:DIM), tofrom: x(it)%ptr(:))
      do i = 1, DIM
        x(i)%ptr(1) = i
      end do
    !$omp end target data
  end subroutine
end module

! { dg-final { scan-tree-dump "if \\(it <= 17\\) goto <D\\\.\[0-9\]+>; else goto <D\\\.\[0-9\]+>;" "omplower" } }
! { dg-final { scan-tree-dump "map\\(iterator\\(integer\\(kind=\[0-9\]+\\) it=1:17:1, loop_label=<D\\\.\[0-9\]+>, elems=omp_iter_data\\\.\[0-9\]+, index=D\\\.\[0-9\]+\\):to:\\(\\\*x\\\.\[0-9\]+\\)\\\[D\\\.\[0-9\]+\\\]\\\.ptr" "omplower" } }
! { dg-final { scan-tree-dump "map\\(iterator\\(integer\\(kind=\[0-9\]+\\) it=1:17:1, loop_label=<D\\\.\[0-9\]+>, elems=omp_iter_data\\\.\[0-9\]+, index=D\\\.\[0-9\]+\\):tofrom:MEM" "omplower" } }
! { dg-final { scan-tree-dump "map\\(iterator\\(integer\\(kind=\[0-9\]+\\) it=1:17:1, loop_label=<D\\\.\[0-9\]+>, elems=omp_iter_data\\\.\[0-9\]+, index=D\\\.\[0-9\]+\\):attach:\\(\\\*x\\\.\[0-9\]+\\)\\\[D\\\.\[0-9\]+\\\]\\\.ptr.data" "omplower" } }
