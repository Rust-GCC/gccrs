! { dg-do run }
! { dg-additional-sources submodule_private_host_aux.f90 }
!
! PR fortran/125430
!
! Module-contained procedures with PRIVATE access must retain global ELF
! linkage so that submodules compiled as separate translation units can
! reach them via host association.

module m
  implicit none
  private
  public :: pub

  interface
    module subroutine pub(x)
      double precision, intent(out) :: x
    end subroutine
  end interface

contains

  pure function priv(n) result(x)
    integer, intent(in) :: n
    double precision :: x
    x = dble(n) * 0.5d0
  end function

end module
