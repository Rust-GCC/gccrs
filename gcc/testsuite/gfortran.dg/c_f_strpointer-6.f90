! { dg-do compile }
! Test that lack of target attribute and wrong-length character array are
! rejected.
program test

  use iso_c_binding
  implicit none

  character (kind=c_char, len=1), dimension(15) :: a
  character (kind=c_char, len=4), dimension(15), target :: b
  character (len=:, kind=c_char), pointer :: fp

  a = [ character :: 'h', 'e', 'l', 'l', 'o', ' ', &
                     'w', 'o', 'r', 'l', 'd', '!', &
                     ' ', ' ', ' ']
  a(13) = C_NULL_CHAR
  b = a
  call c_f_strpointer (a, fp)  ! { dg-error ".cstrarray. argument of .c_f_strpointer. intrinsic at .1. shall have the TARGET attribute" }
  call c_f_strpointer (b, fp)  ! { dg-error ".cstrarray. argument of .c_f_strpointer. intrinsic at .1. shall be a rank-one character array of kind C_CHAR and character length one" }
end program
