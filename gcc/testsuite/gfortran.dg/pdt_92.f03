! { dg-do compile }
! { dg-options "-fdump-tree-original" }
!
! Test the fix for pr125669, in which the components 'v' and 'c' below were
! allocated and freed unnecessarily.
!
module m
  implicit none
  type :: t(k1, k2)
    integer, kind :: k1, k2
    real :: v(k1*k2, max(k1,k2))
    character(len = k1+k2-1) :: c
  end type
end module

program p
  use m
  implicit none
  integer, parameter :: n=3, k=2
  type(t(n,k)) :: x
  if (any (shape(x%v) /= [6,3])) stop 1
  if (len(x%c) /= 4) stop 2
end program
! { dg-final { scan-tree-dump-times "__builtin_malloc" 0 "original" } }
! { dg-final { scan-tree-dump-times "__builtin_free" 0 "original" } }
