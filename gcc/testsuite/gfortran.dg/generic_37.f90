! { dg-do run }
!
! PR fortran/125481
!
! Wrong generic resolution when actual argument is a procedure pointer and
! the generic has two specific procedures -- one with a procedure-pointer
! dummy argument and one with a REAL(8) dummy argument.  gfortran was
! selecting the REAL(8) specific (F23:15.5.2.5, para 2 violation).

module m
  implicit none

  abstract interface
    pure function init_i (x) result (y)
      real (8), intent (in) :: x(:)
      real (8), allocatable :: y(:)
    end function
  end interface

  type :: t
    real (8), allocatable :: vals(:)
  end type

  interface make_t
    module procedure make_t_from_func   ! first dummy: procedure pointer
    module procedure make_t_constant    ! first dummy: real(8)
  end interface

contains

  function make_t_from_func (f, n) result (r)
    procedure (init_i), pointer :: f
    integer, intent (in) :: n
    type (t) :: r
    integer :: i
    r%vals = f ([(real (i, 8), i = 1, n)])
  end function

  function make_t_constant (c, n) result (r)
    real (8), intent (in) :: c
    integer, intent (in) :: n
    type (t) :: r
    integer :: i
    r%vals = [(c, i = 1, n)]
  end function

  pure function identity (x) result (y)
    real (8), intent (in) :: x(:)
    real (8), allocatable :: y(:)
    y = x
  end function

end module m

program test
  use m
  implicit none
  procedure (init_i), pointer :: f => identity
  type (t) :: x
  integer :: i

  x = make_t (f, 4)
  do i = 1, 4
    if (abs (x%vals(i) - real (i, 8)) > epsilon (x%vals(i))) STOP 1
  end do

  x = make_t (42.0d0, 4)
  if (any (abs (x%vals - 42.0d0) > epsilon (x%vals(1)))) stop 2

end program test
