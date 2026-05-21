! { dg-do compile }
! PR fortran/125379 - this gave an ICE due to C binding private
! globals.
! Test case by Juergen Reuter.

module blha_olp_interfaces
  use, intrinsic :: iso_c_binding !NODEP!                                                                                                                                                                     
  use, intrinsic :: iso_fortran_env
  implicit none
  private
  public :: olp_polvec
  type :: blha_driver_t
    procedure(olp_polvec), nopass, pointer :: blha_olp_polvec => null ()
  end type blha_driver_t

  interface
    subroutine olp_polvec (eps) bind(C)
      import
      real(kind = c_double), dimension(0:7), intent(out) :: eps
    end subroutine
  end interface
end module blha_olp_interfaces


module pcm_base
  use blha_olp_interfaces
  implicit none
  private
end module pcm_base


module api
  use pcm_base
  implicit none
  private
  public :: whizard_api_t

  type :: whizard_api_t
     private
     character(:), allocatable :: logfile
  end type whizard_api_t

end module api

function whizard_get_char (whizard_handle) result (stat) bind (C)
  use iso_c_binding, only: c_ptr  !NODEP!                                                                                                                                                                     
  use iso_c_binding, only: c_f_pointer  !NODEP!                                                                                                                                                               
  use api, only: whizard_api_t
  implicit none
  integer :: stat
  type(c_ptr), intent(in) :: whizard_handle
  type(whizard_api_t), pointer :: whizard

  call c_f_pointer (whizard_handle, whizard)

end function whizard_get_char
