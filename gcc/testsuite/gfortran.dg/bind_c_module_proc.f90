! { dg-do compile }
! PR fortran/79330
! Verify that named constants from the host scope are accessible
! in module procedure interface bodies for bind(C, name=...).

module m
  implicit none
  character(len=*), parameter :: PREFIX = "_gfortran_"
  interface
    module subroutine sub() bind(C, name=PREFIX//"caf_sub")
      implicit none
    end subroutine
    module function func() result(r) bind(C, name=PREFIX//"caf_func")
      implicit none
      integer :: r
    end function
  end interface
end module
