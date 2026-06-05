! { dg-do run }
! Test that a nested implied-do array constructor with a derived-type
! constructor whose argument uses an outer loop variable produces correct
! results when the derived type has an allocatable component.
!
! PR fortran/125535

module m
  implicit none
  type :: t
    real, allocatable :: v(:)
  end type
contains
  pure function make(x) result(r)
    real, intent(in) :: x(:)
    type(t) :: r
    r%v = x
  end function
end module

program implied_do_alloc_comp
  use m
  implicit none
  integer, parameter :: n=3, k=2
  type(t), allocatable :: a(:,:)
  real :: h(k, n)
  integer :: i, j

  h(1,:) = [10., 20., 30.]
  h(2,:) = [11., 21., 31.]

  ! Nested implied-do where inner DT constructor uses outer variable i.
  ! Without the fix this produces heap-use-after-free and wrong values.
  a = reshape([([(make(h(:,i)), j=1,1)], i=1,n)], [1,n])

  if (any(a(1,1)%v /= h(:,1))) stop 1
  if (any(a(1,2)%v /= h(:,2))) stop 2
  if (any(a(1,3)%v /= h(:,3))) stop 3
end program
