! { dg-do run }
! Test that c_f_strpointer works with strings/arrays of known length but
! no null terminator character.

module mod
use iso_c_binding
implicit none(type, external)
type t
  type(c_ptr) :: cptr
  character(1, c_char) :: carr(10)
end type t
contains
subroutine sub(x, y)
  type(t), target :: x
  character, pointer, contiguous, intent(in) :: y(:)
  character(:), pointer :: fstr

  call c_f_strpointer (x%cptr, fstr, 10)
  print *, len(fstr), fstr
  if (len(fstr) /= 10 .or. fstr /= "1234567890") stop 1

  call c_f_strpointer (x%carr, fstr)
  print *, len(fstr), fstr
  if (len(fstr) /= 10 .or. fstr /= "abcdefghij") stop 2

  call c_f_strpointer (y, fstr)
  if (len(fstr) /= 10 .or. fstr /= "abcdefghij") stop 3

  call c_f_strpointer (y(5:), fstr)
  if (len(fstr) /= 6 .or. fstr /= "efghij") stop 4

  call c_f_strpointer (x%carr(2:4), fstr)
  if (len(fstr) /= 3 .or. fstr /= "bcd") stop 5
end
end module

use mod
implicit none
character(10,c_char), target :: str10
character(1,c_char), target :: arr10(10)

type(t) :: arg

str10 = '1234567890'
arr10 = ['a','b','c','d','e','f','g','h','i', 'j']

arg%cptr = c_loc(str10)
arg%carr = arr10
call sub(arg, arr10)
end
