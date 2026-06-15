! { dg-do run }
! PR libfortran/114618 - Test TL, TR, and T specifiers in combination for
! sequential and stream formatted file I/O.
program fmt_t_11
  implicit none
  integer :: u
  character(len=20) :: line

  ! --- Sequential formatted file tests ---

  ! T backward then TR forward into already-written content
  open (newunit=u, status='scratch', form='formatted')
  write (u, '(10("X"),t3,"AB",tr3,"C")')
  rewind (u)
  read (u, '(a)') line
  close (u)
  if (line(1:10) /= 'XXABXXXCXX') stop 1

  ! TL backward then TR forward into already-written content
  open (newunit=u, status='scratch', form='formatted')
  write (u, '("HELLO123",tl5,"AB",tr3,"Z")')
  rewind (u)
  read (u, '(a)') line
  close (u)
  if (line(1:9) /= 'HELAB123Z') stop 2

  ! T, TL, and TR all combined in one format
  open (newunit=u, status='scratch', form='formatted')
  write (u, '(t7,"*",tl5,"AB",tr3,"Z")')
  rewind (u)
  read (u, '(a)') line
  close (u)
  if (line(1:8) /= '  AB  *Z') stop 3

  ! --- Stream formatted file tests ---

  ! T backward then TR forward into already-written content
  open (newunit=u, status='scratch', form='formatted', access='stream')
  write (u, '(10("X"),t3,"AB",tr3,"C")')
  rewind (u)
  read (u, '(a)') line
  close (u)
  if (line(1:10) /= 'XXABXXXCXX') stop 4

  ! TL backward then TR forward into already-written content
  open (newunit=u, status='scratch', form='formatted', access='stream')
  write (u, '("HELLO123",tl5,"AB",tr3,"Z")')
  rewind (u)
  read (u, '(a)') line
  close (u)
  if (line(1:9) /= 'HELAB123Z') stop 5

  ! T, TL, and TR all combined in one format
  open (newunit=u, status='scratch', form='formatted', access='stream')
  write (u, '(t7,"*",tl5,"AB",tr3,"Z")')
  rewind (u)
  read (u, '(a)') line
  close (u)
  if (line(1:8) /= '  AB  *Z') stop 6

end program fmt_t_11
