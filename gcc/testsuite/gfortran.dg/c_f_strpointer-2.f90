! { dg-do run }
program test

  use iso_c_binding
  implicit none

  character (kind=c_char, len=1), dimension(15), target :: a

  a = [ character :: 'h', 'e', 'l', 'l', 'o', ' ', &
                     'w', 'o', 'r', 'l', 'd', '!', &
                     ' ', ' ', ' ']
  a(13) = C_NULL_CHAR
  call doit (a, 12, 15)
  call doit (a(7:), 6, 9)

contains

subroutine doit (aa, n, m)
  character (kind=c_char, len=1), dimension(*), target, intent(inout) :: aa
  integer, intent(in) :: n, m
  character (len=:, kind=c_char), pointer :: fp

  ! check length is correct
  call c_f_strpointer (aa, fp, m)
  if (len (fp) .ne. n) stop 100

  ! check that fp points to the contents of array aa.
  if (fp(1:1) .ne. aa(1)) stop 101
  aa(1) = '?'
  if (fp(1:1) .ne. '?') stop 102
end subroutine
     
end program
