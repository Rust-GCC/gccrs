! { dg-do run }
!
! Test the fix for PR117077. The test has been made more elaborate that the
! original to demonstrate that the hidden derived type can be used as a container
! for returning processed/verified versions of a number of variables.
!
! Contributed by Ivan Pribec  <ivan.pribec@gmail.com>
!
module hidden
  implicit none
  private
  public :: foo
contains
  type(foo_type) function foo(a1, a2, a3) result(f)
    integer, intent(in) :: a1
    real(kind(1.0d0)), intent(in) :: a2
    real(kind(1.0e0)), intent(in) :: a3(3)
    type :: foo_type
      integer :: first
      real(kind(1.0d0)) :: second
      real, allocatable :: third(:)
    end type
    f = foo_type(a1 - 1, a2 / 2.0, a3 * 2)
  end function
end module

program main
  use hidden
  type :: main_type
    integer :: first
    real(kind(1.0d0)) :: second
    real, allocatable :: third(:)
  end type

  integer :: a1 = 42
  real(kind(1.0d0)) :: a2 = 5.0d0
  real(kind(1.0e0)) :: a3(3) = [1.0,2.0,3.0]
  type(main_type) :: g

  associate(f => foo(a1, a2, a3)) ! ICE in the assignment of foo result to f
    if (f%first /= a1 - 1) stop 1
    if (int (f%second) /= int(a2 / 2.0)) stop 2
    if (kind (f%second) /= kind (a2)) stop 3
    if (.not.allocated(f%third)) stop 4
    if (any (abs (f%third - a3 * 2) > 1e-6)) stop 5
  end associate

  g = transfer (foo(a1, a2, a3), mold = g)
  if (g%first /= a1 - 1) stop 11
  if (int (g%second) /= int(a2 / 2.0)) stop 12
  if (kind (g%second) /= kind (a2)) stop 13
  if (.not.allocated(g%third)) stop 14
  if (any (abs (g%third - a3 * 2) > 1e-6)) stop 15

  deallocate (g%third)
end program
