! { dg-do compile }
! Test that missing size argument is rejected.

program test

  use iso_c_binding
  implicit none

  character (kind=c_char, len=1), dimension(15), target :: a

  a = [ character :: 'h', 'e', 'l', 'l', 'o', ' ', &
                     'w', 'o', 'r', 'l', 'd', '!', &
                     ' ', ' ', ' ']
  a(13) = C_NULL_CHAR
  call doit (a, 12)

contains

subroutine doit (aa, n)
  character (kind=c_char, len=1), dimension(*), target, intent(inout) :: aa
  integer, intent(in) :: n
  character (len=:, kind=c_char), pointer :: fp
  type(c_ptr) :: p

  p = C_LOC (aa(1))
  call c_f_strpointer (p, fp)  ! { dg-error ".nchars. argument of .c_f_strpointer. intrinsic shall be present when the .cstrptr. argument at .1. is a C_PTR" }
  call c_f_strpointer (aa, fp) ! { dg-error ".nchars. argument of .c_f_strpointer. intrinsic shall be present when the .cstrarray. argument at .1. is assumed-size" }

  ! These are all OK, they are known-size array sections of the assumed-size
  ! array aa.
  call c_f_strpointer (aa(:10), fp)
  call c_f_strpointer (aa(:huge(1)), fp)
  call c_f_strpointer (aa(5:10), fp)

end subroutine

end program
