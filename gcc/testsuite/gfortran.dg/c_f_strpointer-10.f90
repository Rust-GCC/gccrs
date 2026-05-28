! { dg-do run }
! Check handling of optional dummy nchars argument to c_f_strpointer
! when its corresponding actual argument is an optional dummy that is
! not present, and the C string argument has no size information (C 
! pointer or assumed-size array).
! The Fortran spec says this is not allowed, but it's a runtime error
! and the gfortran implementation assumes size 0 in this case rather than
! diagnosing it.

program test

  use iso_c_binding
  implicit none

  character (kind=c_char, len=1), dimension(15), target :: a

  a = [ character :: 'h', 'e', 'l', 'l', 'o', ' ', &
                     'w', 'o', 'r', 'l', 'd', '!', &
                     ' ', ' ', ' ']
  a(13) = C_NULL_CHAR
  call doit (a, 12, 15)
  call doit (a, 0)
contains

subroutine doit (aa, n, m)
  character (kind=c_char, len=1), dimension(*), target, intent(inout) :: aa
  integer, intent(in) :: n
  integer, intent(in), optional :: m
  character (len=:, kind=c_char), pointer :: fp
  type(c_ptr) :: p

  p = C_LOC (aa(1))
  call c_f_strpointer (p, fp, m)
  if (len(fp) .ne. n) stop 100
  call c_f_strpointer (aa, fp, m)
  if (len(fp) .ne. n) stop 200
end subroutine

end program
