! { dg-do compile }
! { dg-options "" }
!
! PR fortran/125531
! ASSOCIATE with a contained-function selector, using the result's data
! components as arguments to another function (also from CONTAINS), where
! the result type is defined in program scope.  This used to fail with
! "Invalid association target".
!
program test
  implicit none

  type :: args_t
    integer :: order_ = 4
    integer :: cells_ = 20
    double precision :: x_min_ = 0d0, x_max_ = 1d0
  end type

  associate(args => get_args())
    associate(result => compute(args%order_, args%cells_))
      print *, result
    end associate
  end associate

contains

  function get_args() result(r)
    type(args_t) :: r
    r%order_ = 2
    r%cells_ = 10
  end function

  function compute(order, cells) result(r)
    integer, intent(in) :: order, cells
    integer :: r
    r = order * cells
  end function

end program
