! { dg-do compile }
! Test that c_f_strpointer rejects assumed-rank array argument.

subroutine sub(y)
  use iso_c_binding
  implicit none (type, external)
  character, pointer, contiguous, intent(in) :: y(..)
  character(:), pointer :: fstr

  call c_f_strpointer (y, fstr, 10) ! { dg-error "Assumed-rank argument at .1. is only permitted as actual argument to intrinsic inquiry functions or to RESHAPE." }
end
