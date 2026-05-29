! { dg-do compile }
! { dg-options "-O2 -fno-inline-small-functions -fno-inline-functions -fno-inline-functions-called-once -fdump-tree-optimized" }
!
! With all automatic inlining disabled the helper procedure would not be
! inlined, but the INLINE attribute marks it as declared-inline, so it is
! inlined anyway and no standalone call remains.

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
!GCC$ ATTRIBUTES inline :: helper
    integer :: i
    do i = 1, m
      x(i) = x(i) + 1.0
    end do
  end subroutine helper
end subroutine caller

! { dg-final { scan-tree-dump-not "helper" "optimized" } }
