! { dg-do compile }
! { dg-options "-O -fdump-tree-optimized" }
!
! Verify that !GCC$ ATTRIBUTES always_inline forces inlining: the helper
! procedure is inlined into its caller and no standalone call remains.

subroutine caller(a, n)
  implicit none
  integer, intent(in) :: n
  real, intent(inout) :: a(n)
  call helper(a, n)
  call helper(a, n)
contains
  subroutine helper(x, m)
    implicit none
    integer, intent(in) :: m
    real, intent(inout) :: x(m)
!GCC$ ATTRIBUTES always_inline :: helper
    integer :: i
    do i = 1, m
      x(i) = x(i) + 1.0
    end do
  end subroutine helper
end subroutine caller

! { dg-final { scan-tree-dump-not "helper" "optimized" } }
