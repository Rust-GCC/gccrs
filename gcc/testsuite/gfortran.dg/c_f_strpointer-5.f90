! { dg-do compile }
! Test that multi-dimensional array arguments are rejected.
program test

  use iso_c_binding
  implicit none

  character (kind=c_char, len=1), target :: a(15), b(3,5)
  character (len=:, kind=c_char), pointer :: fp

  a = [ character :: 'h', 'e', 'l', 'l', 'o', ' ', &
                     'w', 'o', 'r', 'l', 'd', '!', &
                     ' ', ' ', ' ']
  a(13) = C_NULL_CHAR
  b = reshape (a, shape (b))

  call c_f_strpointer (a, fp)
  call c_f_strpointer (b, fp)  ! { dg-error ".cstrarray. argument of .c_f_strpointer. intrinsic at .1. shall be a rank-one character array of kind C_CHAR and character length one" }
end program
