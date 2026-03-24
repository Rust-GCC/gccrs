! { dg-do compile }
!
! PR fortran/124567 - overriding method not resolved correctly
!
! Contributed by Salvatore Filippone 

module psb_base_spm_mod
  type  :: psb_base_spm
  end type psb_base_spm
end module psb_base_spm_mod

module psb_r_base_spm_mod
  use psb_base_spm_mod
  type, extends(psb_base_spm) :: psb_r_base_spm
  contains
    procedure, pass(a) :: csgrw => psb_r_base_csgrw
  end type psb_r_base_spm

  interface
    subroutine psb_r_base_csgrw(a,iren)
      import
      class(psb_r_base_spm), intent(in) :: a
      integer, intent(in), optional     :: iren(:)
    end subroutine psb_r_base_csgrw
  end interface
end module psb_r_base_spm_mod

module psb_d_mf_mat_mod
  use psb_r_base_spm_mod
  type, extends(psb_r_base_spm) :: psb_d_mf_spm
    procedure(d_mf_mv), pass(a), pointer :: var_csmv => null()
  contains
    procedure, pass(a) :: csgrw     => psb_d_mf_csgrw
    procedure, pass(a) :: set_csmv  => d_mf_set_csmv
  end type psb_d_mf_spm

  interface
    subroutine d_mf_mv(a,x,info)
      import :: psb_d_mf_spm
      class(psb_d_mf_spm), intent(in) :: a
      real, intent(in)                :: x(:)
      integer, intent(out)            :: info
    end subroutine d_mf_mv
  end interface

  interface
    subroutine psb_d_mf_csgrw(a,iren)
      import
      class(psb_d_mf_spm), intent(in) :: a
      integer, intent(in), optional   :: iren(:)
    end subroutine psb_d_mf_csgrw
  end interface

contains
  subroutine  d_mf_set_csmv(func,a)
    implicit none
    class(psb_d_mf_spm), intent(inout) :: a
    procedure(d_mf_mv)                 :: func
    a%var_csmv => func
    return
  end subroutine d_mf_set_csmv
end module psb_d_mf_mat_mod
