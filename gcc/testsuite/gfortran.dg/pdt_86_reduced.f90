! { dg-do compile }
! { dg-options "-fdump-tree-original" }
!
! Check the fix for the memory leak in pdt_77.f03. This test is a reduced
! version of the original that isolates the source of the leak. Note that
! the leak is not PDT specific.
!
module input_output_pair_m
  implicit none
  type tensor_t                             ! No longer a PDT!
    real(kind(1e0)), allocatable :: values_(:)
  end type
end module

  use input_output_pair_m
  call test
contains
  subroutine test
    implicit none
    type(tensor_t), allocatable :: inputs(:)
    integer :: i
    inputs = [(tensor_t([real(i)]), i=1,2)] ! Leaked 8 bytes in 2 blocks.
  end
end
! { dg-final { scan-tree-dump-times "__builtin_malloc" 7 "original" } }
! { dg-final { scan-tree-dump-times "__builtin_free" 8 "original" } }
