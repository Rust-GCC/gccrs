! { dg-do run }
!
! PR fortran/125650
!
! Verify that an ASSOCIATE selector that is an overloaded intrinsic operator
! expression (here a CHARACTER-returning `+', bound via an explicit
! interface) is extended to a function call early enough that the
! associate name receives a known type before the body of the ASSOCIATE
! construct is parsed.  Previously the associate name stayed untyped,
! a substring reference to it was matched incorrectly, and the wrong
! reference reached the back end, causing an ICE in trans-expr.cc.

module associate_81_m
  implicit none

  interface operator(+)
    module procedure concat
  end interface

contains

  function concat (dd1, dd2)
    implicit none
    character(len = 8) :: concat
    character(len = 3), intent(in) :: dd1
    character(len = 5), intent(in) :: dd2
    concat = dd1 // dd2
  end function

end module

program associate_81
  use associate_81_m
  implicit none

  character(len = 3) :: arg1 = 'aaa'
  character(len = 5) :: arg2 = 'ccccc'

  associate (aa => arg1, bb => arg2, cc => arg1 + arg2)
    if (cc /= 'aaaccccc') stop 1
    if (cc(3:5) /= 'acc') stop 2
  end associate

end program
