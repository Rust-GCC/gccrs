! { dg-do run }
! PR fortran/82086 - namelist read of derived-type array component
! with a repeat count must set all addressed elements.
program pr82086
  implicit none
  type t
     character(64) :: c = ''
  end type t
  type(t), dimension(16) :: ta
  integer :: i
  namelist /n/ ta

  open (10, status="scratch", delim="apostrophe")
  write (10, '(a)') "&n"
  write (10, '(a)') " ta(1:8)%c = 8*'bogus'"
  write (10, '(a)') "/"
  rewind (10)
  read (10, nml=n)
  close (10)

  do i = 1, 8
    if (ta(i)%c /= 'bogus') stop i
  end do
  do i = 9, 16
    if (ta(i)%c /= '') stop 10+i
  end do
end program pr82086
