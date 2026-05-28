! { dg-do compile }
! Test that problems with the fstrptr argument are diagnosed.

program test

  use iso_c_binding
  implicit none

  character (kind=c_char, len=1), dimension(15), target :: a
  character (len=:, kind=c_char), pointer :: fp

  a = [ character :: 'h', 'e', 'l', 'l', 'o', ' ', &
                     'w', 'o', 'r', 'l', 'd', '!', &
                     ' ', ' ', ' ']
  a(13) = C_NULL_CHAR
  call doit (a, 12, fp)

contains

subroutine doit (aa, n, fp1)
  character (kind=c_char, len=1), dimension(*), target, intent(inout) :: aa
  integer, intent(in) :: n
  character (len=:, kind=c_char), pointer, intent(in) :: fp1
  character (len=42, kind=c_char), pointer :: fp2
  character (len=:, kind=c_char), allocatable :: fp3
  type(c_ptr) :: p

  p = C_LOC (aa(1))
  call c_f_strpointer (p, fp1, n) ! { dg-error ".fstrptr. argument of .c_f_strpointer. intrinsic at .1. cannot be INTENT.IN." }
  call c_f_strpointer (p, fp2, n) ! { dg-error ".fstrptr. argument of .c_f_strpointer. intrinsic at .1. shall be a scalar deferred-length character pointer of kind C_CHAR" }
  call c_f_strpointer (p, fp3, n) ! { dg-error ".fstrptr. argument of .c_f_strpointer. intrinsic at .1. shall be a scalar deferred-length character pointer of kind C_CHAR" }
end subroutine

end program
