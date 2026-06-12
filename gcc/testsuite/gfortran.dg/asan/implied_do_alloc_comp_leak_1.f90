!{ dg-do run }

! PR fortran/125535

! An array constructor with an implied-do whose element is a function result
! of derived type with allocatable components moves the result into the
! constructor temporary, which then owns those components.  The per-element
! finalization only freed the last slot written by the loop, leaking the
! allocatable components of every other element.  Checked under -fsanitize=address.

module m
  implicit none
  type :: t
    real, allocatable :: v(:)
  end type
contains
  pure function make (x) result (r)
    real, intent(in) :: x(:)
    type(t) :: r
    r%v = x
  end function
end module

program implied_do_alloc_comp_leak_1
  use m
  implicit none
  integer, parameter :: n = 4
  type(t), allocatable :: a(:), b(:,:)
  real :: h(2, n)
  integer :: i, j

  do i = 1, n
    h(:, i) = [real(i), real(2 * i)]
  end do

  ! Flat implied-do of function results.
  a = [(make (h(:, i)), i = 1, n)]
  if (any (a(n)%v /= h(:, n))) stop 1
  deallocate (a)

  ! Nested implied-do feeding a transformational intrinsic.
  b = reshape ([([(make (h(:, i)), j = 1, 1)], i = 1, n)], [1, n])
  if (any (b(1, 2)%v /= h(:, 2))) stop 2
  deallocate (b)
end program
