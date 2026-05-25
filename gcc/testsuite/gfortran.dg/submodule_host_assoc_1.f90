! { dg-do run }
! { dg-additional-sources submodule_host_assoc_1_aux.f90 }
!
! PR fortran/125527
!
! A submodule must be able to access internal subprograms of its ancestor
! module via host association (Fortran 2018, 14.6.1.3).
! gfortran was incorrectly rejecting such references with
! "has no IMPLICIT type" when the internal procedure was declared PRIVATE.

module m
  implicit none
  private
  public :: test

  interface
    module subroutine test()
    end subroutine
  end interface

contains
  pure function double_it(x) result(y)
    double precision, intent(in) :: x
    double precision :: y
    y = 2.0d0 * x
  end function

end module m
