! { dg-do run }
! This is the example from the manual.

program main
  use iso_c_binding, only: f_c_string, c_char
  implicit none (external, type)
  character(:, c_char), allocatable :: s1, s2, s3

  ! s1 is null-terminated "hello, world!   "
  s1 = f_c_string ("hello, world!   ", .true.)
  if (len(s1) .ne. 17) stop 100

  ! s2 is null-terminated "hello, world!"
  s2 = f_c_string ("hello, world!   ", .false.)
  if (len(s2) .ne. 14) stop 200

  ! s3 is null-terminated "hello, world!" (same as s2 example)
  s3 = f_c_string ("hello, world!   ")
  if (len(s3) .ne. 14) stop 200
end program main
