! { dg-do compile }
! Test that non-contiguous array section argument is rejected.
program test

  use iso_c_binding
  implicit none

  character (kind=c_char, len=1), dimension(15), target :: a
  character (len=:, kind=c_char), pointer :: fp

  a = [ character :: 'h', 'e', 'l', 'l', 'o', ' ', &
                     'w', 'o', 'r', 'l', 'd', '!', &
                     ' ', ' ', ' ']
  a(13) = C_NULL_CHAR

  call c_f_strpointer (a(1:13), fp)
  call c_f_strpointer (a(1:13:2), fp)  ! { dg-error ".cstrarray. argument of .c_f_strpointer. intrinsic at .1. shall be simply contiguous" }
end program
