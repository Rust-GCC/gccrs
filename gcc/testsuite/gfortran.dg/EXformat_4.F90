! { dg-do run }
! PR93727 Test EX format READ (read_ex).
! Covers: hex-significand input (0X...P...), decimal fallback, round-trips,
! INF/NAN, and all supported real kinds.
program main
  implicit none
  call test04
  call test08
  call test10
  call test16

contains

subroutine test04
  real(kind=4) :: r4
  character(kind=1,len=40) :: s

  ! Round-trip: write then read back.
  s = ' '
  write(s,'(EX0.0)') -huge(1.0_4)
  read(s,'(EX40.0)') r4
  if (r4 /= -huge(1.0_4)) stop 1

  s = ' '
  write(s,'(EX0.0)') tiny(1.0_4)
  read(s,'(EX40.0)') r4
  if (r4 /= tiny(1.0_4)) stop 2

  ! Explicit hex literal input.
  s = '0X1.8P+0'             ! = 1.5 exactly
  read(s,'(EX40.0)') r4
  if (r4 /= 1.5_4) stop 3

  s = '-0X1.P+0'             ! = -1.0
  read(s,'(EX40.0)') r4
  if (r4 /= -1.0_4) stop 4

  ! Decimal fallback: no 0X prefix, ordinary E-style input.
  s = '1.5E0'
  read(s,'(EX40.0)') r4
  if (r4 /= 1.5_4) stop 5

  s = '   1.5E2   '
  read(s,'(EX40.0)') r4
  if (r4 /= 150.0_4) stop 6

  ! Zero field (blank record).
  s = '    '
  read(s,'(EX4.0)') r4
  if (r4 /= 0.0_4) stop 7

end subroutine test04


subroutine test08
  real(kind=8) :: r8
  character(kind=1,len=40) :: s

  ! Round-trip.
  s = ' '
  write(s,'(EX0.0)') -huge(1.0_8)
  read(s,'(EX40.0)') r8
  if (r8 /= -huge(1.0_8)) stop 8

  s = ' '
  write(s,'(EX0.0)') tiny(1.0_8)
  read(s,'(EX40.0)') r8
  if (r8 /= tiny(1.0_8)) stop 9

  ! Explicit hex: IEEE 754 representation of 1/3.
  s = '0X1.5555555555555P-2'
  read(s,'(EX40.0)') r8
  if (r8 /= 1.0_8 / 3.0_8) stop 10

  ! Negative hex.
  s = '-0X1.8P+0'            ! = -1.5
  read(s,'(EX40.0)') r8
  if (r8 /= -1.5_8) stop 11

  ! Decimal fallback.
  s = '3.14159265358979E0'
  read(s,'(EX40.0)') r8
  if (abs(r8 - 3.14159265358979_8) > 2.0_8 * epsilon(r8)) stop 12

  ! Round-trip on an interesting value.
  s = ' '
  write(s,'(EX0.0)') -3.14159265358979_8 * 25.0_8
  read(s,'(EX40.0)') r8
  if (r8 /= -3.14159265358979_8 * 25.0_8) stop 13

  ! Lowercase hex prefix and negative binary exponent.
  s = '0x1.8p-1'            ! = 0.75
  read(s,'(EX40.0)') r8
  if (r8 /= 0.75_8) stop 14

  ! d-field: no decimal point in decimal input; d=2 places from right.
  s = '12345   '
  read(s,'(EX10.2)') r8
  if (r8 /= 123.45_8) stop 15

  ! INF and NaN input via decimal fallback path.
  s = 'Inf'
  read(s,'(EX40.0)') r8
  if (r8 <= huge(r8)) stop 16

  s = '-Infinity'
  read(s,'(EX40.0)') r8
  if (r8 >= -huge(r8)) stop 17

  s = 'NaN'
  read(s,'(EX40.0)') r8
  if (r8 == r8) stop 18

end subroutine test08


#ifdef __GFC_REAL_10__
subroutine test10
  real(kind=10) :: r10
  character(kind=1,len=40) :: s

  ! Round-trip.
  s = ' '
  write(s,'(EX0.0)') -huge(1.0_10)
  read(s,'(EX40.0)') r10
  if (r10 /= -huge(1.0_10)) stop 19

  s = ' '
  write(s,'(EX0.0)') tiny(1.0_10)
  read(s,'(EX40.0)') r10
  if (r10 /= tiny(1.0_10)) stop 20

  ! Explicit hex: 1.5 = 0X1.8P+0
  s = '0X1.8P+0'
  read(s,'(EX40.0)') r10
  if (r10 /= 1.5_10) stop 21

end subroutine test10
#else
subroutine test10
end subroutine test10
#endif


#ifdef __GFC_REAL_16__
subroutine test16
  real(kind=16) :: r16
  character(kind=1,len=40) :: s

  ! Round-trip.
  s = ' '
  write(s,'(EX0.0)') -huge(1.0_16)
  read(s,'(EX40.0)') r16
  if (r16 /= -huge(1.0_16)) stop 22

  s = ' '
  write(s,'(EX0.0)') tiny(1.0_16)
  read(s,'(EX40.0)') r16
  if (r16 /= tiny(1.0_16)) stop 23

  ! Explicit hex: 1.5 = 0X1.8P+0
  s = '0X1.8P+0'
  read(s,'(EX40.0)') r16
  if (r16 /= 1.5_16) stop 24

end subroutine test16
#else
subroutine test16
end subroutine test16
#endif

end program main
