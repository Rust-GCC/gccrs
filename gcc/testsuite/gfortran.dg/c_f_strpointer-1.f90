! { dg-do run }
program test

  use iso_c_binding
  implicit none

  character (kind=c_char, len=1), dimension(15), target :: a
  type(c_ptr) :: p
  character (len=:, kind=c_char), pointer :: fp1, fp2

  a = [ character :: 'h', 'e', 'l', 'l', 'o', ' ', &
                     'w', 'o', 'r', 'l', 'd', '!', &
                     ' ', ' ', ' ']
  a(13) = C_NULL_CHAR
  p = C_LOC (a(1))

  ! check length is correct
  call c_f_strpointer (p, fp1, 15)
  if (len (fp1) .ne. 12) stop 100
  call c_f_strpointer (a, fp2)
  if (len (fp2) .ne. 12) stop 101

  ! check that fp1 and fp2 both point to the contents of array a.
  if (fp1(1:1) .ne. 'h') stop 200
  if (fp2(1:1) .ne. 'h') stop 201
  a(1) = 'H'
  if (fp1(1:1) .ne. 'H') stop 202
  if (fp2(1:1) .ne. 'H') stop 203
     
end program
