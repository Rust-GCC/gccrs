! { dg-do run }
! Test that allocating an array of parameterized derived type initializes
! the allocatable components of ALL elements, not just element 0.
! PR fortran/125534

module m
  implicit none
  integer, parameter :: default_real = kind(0.)
  type :: tensor_t(k)
    integer, kind :: k = default_real
    real(k), allocatable :: values_(:)
  end type
end module

program pdt_array_alloc
  use m
  implicit none
  integer, parameter :: n = 5
  type(tensor_t), allocatable :: a(:)
  integer :: i

  allocate(a(n))
  ! Without the fix, only a(1)%values_ was initialized (nullified);
  ! a(2)..a(n) had garbage pointers and the assignment below would crash.
  do i = 1, n
    a(i)%values_ = [real(i), real(i*2)]
  end do

  if (any(a(3)%values_ /= [3., 6.])) stop 1
end program
