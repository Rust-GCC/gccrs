! { dg-do run }
! PR93727 Test EX format explicit rounding modes (ROUND_NEAREST, ROUND_UP,
! ROUND_DOWN, ROUND_ZERO) for truncated hex mantissa, all supported kinds.
!
! Key values used throughout (all exactly representable):
!   10.736328125   = 0XA.BC8000...P+0  tie at d=2, last kept C (12, even)
!   10.732421875   = 0XA.BB8000...P+0  tie at d=2, last kept B (11, odd)
!   10.736572265625 = 0XA.BC9000...P+0 dropped digit 9 > 8, no tie
program main
  implicit none
  call test04_rounding
  call test08_rounding
  call test10_rounding
  call test16_rounding

contains

! ---------------------------------------------------------------------------
subroutine test04_rounding
  real(kind=4) :: r4
  character(kind=1,len=40) :: s

  ! Positive tie, last kept even (C=12): RN truncates, RU rounds up.
  r4 = 10.736328125_4
  write(s, '(EX0.2)', round='nearest') r4
  if (s(1:9) /= '0XA.BCP+0') stop 1
  write(s, '(EX0.2)', round='up') r4
  if (s(1:9) /= '0XA.BDP+0') stop 2
  write(s, '(EX0.2)', round='down') r4
  if (s(1:9) /= '0XA.BCP+0') stop 3
  write(s, '(EX0.2)', round='zero') r4
  if (s(1:9) /= '0XA.BCP+0') stop 4

  ! Positive tie, last kept odd (B=11): RN rounds up.
  r4 = 10.732421875_4
  write(s, '(EX0.2)', round='nearest') r4
  if (s(1:9) /= '0XA.BCP+0') stop 5

  ! Positive: dropped digit 9 > 8, no tie; all modes with dropped nonzero round up.
  r4 = 10.736572265625_4
  write(s, '(EX0.2)', round='nearest') r4
  if (s(1:9) /= '0XA.BDP+0') stop 6
  write(s, '(EX0.2)', round='up') r4
  if (s(1:9) /= '0XA.BDP+0') stop 7
  write(s, '(EX0.2)', round='zero') r4
  if (s(1:9) /= '0XA.BCP+0') stop 8

  ! Negative tie, last kept even (C=12): RD rounds up in magnitude.
  r4 = -10.736328125_4
  write(s, '(EX0.2)', round='nearest') r4
  if (s(1:10) /= '-0XA.BCP+0') stop 9
  write(s, '(EX0.2)', round='down') r4
  if (s(1:10) /= '-0XA.BDP+0') stop 10
  write(s, '(EX0.2)', round='up') r4
  if (s(1:10) /= '-0XA.BCP+0') stop 11
  write(s, '(EX0.2)', round='zero') r4
  if (s(1:10) /= '-0XA.BCP+0') stop 12

end subroutine test04_rounding

! ---------------------------------------------------------------------------
subroutine test08_rounding
  real(kind=8) :: r8
  character(kind=1,len=40) :: s

  ! Positive tie, last kept even (C=12).
  r8 = 10.736328125_8
  write(s, '(EX0.2)', round='nearest') r8
  if (s(1:9) /= '0XA.BCP+0') stop 13
  write(s, '(EX0.2)', round='up') r8
  if (s(1:9) /= '0XA.BDP+0') stop 14
  write(s, '(EX0.2)', round='down') r8
  if (s(1:9) /= '0XA.BCP+0') stop 15
  write(s, '(EX0.2)', round='zero') r8
  if (s(1:9) /= '0XA.BCP+0') stop 16

  ! Positive tie, last kept odd (B=11): RN rounds up.
  r8 = 10.732421875_8
  write(s, '(EX0.2)', round='nearest') r8
  if (s(1:9) /= '0XA.BCP+0') stop 17

  ! Positive: dropped digit 9 > 8.
  r8 = 10.736572265625_8
  write(s, '(EX0.2)', round='nearest') r8
  if (s(1:9) /= '0XA.BDP+0') stop 18
  write(s, '(EX0.2)', round='up') r8
  if (s(1:9) /= '0XA.BDP+0') stop 19
  write(s, '(EX0.2)', round='zero') r8
  if (s(1:9) /= '0XA.BCP+0') stop 20

  ! Negative tie, last kept even (C=12).
  r8 = -10.736328125_8
  write(s, '(EX0.2)', round='nearest') r8
  if (s(1:10) /= '-0XA.BCP+0') stop 21
  write(s, '(EX0.2)', round='down') r8
  if (s(1:10) /= '-0XA.BDP+0') stop 22
  write(s, '(EX0.2)', round='up') r8
  if (s(1:10) /= '-0XA.BCP+0') stop 23
  write(s, '(EX0.2)', round='zero') r8
  if (s(1:10) /= '-0XA.BCP+0') stop 24

end subroutine test08_rounding

! ---------------------------------------------------------------------------
#ifdef __GFC_REAL_10__
subroutine test10_rounding
  real(kind=10) :: r10
  character(kind=1,len=40) :: s

  ! Positive tie, last kept even (C=12).
  r10 = 10.736328125_10
  write(s, '(EX0.2)', round='nearest') r10
  if (s(1:9) /= '0XA.BCP+0') stop 25
  write(s, '(EX0.2)', round='up') r10
  if (s(1:9) /= '0XA.BDP+0') stop 26
  write(s, '(EX0.2)', round='down') r10
  if (s(1:9) /= '0XA.BCP+0') stop 27
  write(s, '(EX0.2)', round='zero') r10
  if (s(1:9) /= '0XA.BCP+0') stop 28

  ! Positive tie, last kept odd (B=11): RN rounds up.
  r10 = 10.732421875_10
  write(s, '(EX0.2)', round='nearest') r10
  if (s(1:9) /= '0XA.BCP+0') stop 29

  ! Positive: dropped digit 9 > 8.
  r10 = 10.736572265625_10
  write(s, '(EX0.2)', round='nearest') r10
  if (s(1:9) /= '0XA.BDP+0') stop 30
  write(s, '(EX0.2)', round='up') r10
  if (s(1:9) /= '0XA.BDP+0') stop 31
  write(s, '(EX0.2)', round='zero') r10
  if (s(1:9) /= '0XA.BCP+0') stop 32

  ! Negative tie, last kept even (C=12).
  r10 = -10.736328125_10
  write(s, '(EX0.2)', round='nearest') r10
  if (s(1:10) /= '-0XA.BCP+0') stop 33
  write(s, '(EX0.2)', round='down') r10
  if (s(1:10) /= '-0XA.BDP+0') stop 34
  write(s, '(EX0.2)', round='up') r10
  if (s(1:10) /= '-0XA.BCP+0') stop 35
  write(s, '(EX0.2)', round='zero') r10
  if (s(1:10) /= '-0XA.BCP+0') stop 36

end subroutine test10_rounding
#else
subroutine test10_rounding
end subroutine test10_rounding
#endif

! ---------------------------------------------------------------------------
#ifdef __GFC_REAL_16__
subroutine test16_rounding
  real(kind=16) :: r16
  character(kind=1,len=40) :: s

  ! Positive tie, last kept even (C=12).
  r16 = 10.736328125_16
  write(s, '(EX0.2)', round='nearest') r16
  if (s(1:9) /= '0XA.BCP+0') stop 37
  write(s, '(EX0.2)', round='up') r16
  if (s(1:9) /= '0XA.BDP+0') stop 38
  write(s, '(EX0.2)', round='down') r16
  if (s(1:9) /= '0XA.BCP+0') stop 39
  write(s, '(EX0.2)', round='zero') r16
  if (s(1:9) /= '0XA.BCP+0') stop 40

  ! Positive tie, last kept odd (B=11): RN rounds up.
  r16 = 10.732421875_16
  write(s, '(EX0.2)', round='nearest') r16
  if (s(1:9) /= '0XA.BCP+0') stop 41

  ! Positive: dropped digit 9 > 8.
  r16 = 10.736572265625_16
  write(s, '(EX0.2)', round='nearest') r16
  if (s(1:9) /= '0XA.BDP+0') stop 42
  write(s, '(EX0.2)', round='up') r16
  if (s(1:9) /= '0XA.BDP+0') stop 43
  write(s, '(EX0.2)', round='zero') r16
  if (s(1:9) /= '0XA.BCP+0') stop 44

  ! Negative tie, last kept even (C=12).
  r16 = -10.736328125_16
  write(s, '(EX0.2)', round='nearest') r16
  if (s(1:10) /= '-0XA.BCP+0') stop 45
  write(s, '(EX0.2)', round='down') r16
  if (s(1:10) /= '-0XA.BDP+0') stop 46
  write(s, '(EX0.2)', round='up') r16
  if (s(1:10) /= '-0XA.BCP+0') stop 47
  write(s, '(EX0.2)', round='zero') r16
  if (s(1:10) /= '-0XA.BCP+0') stop 48

end subroutine test16_rounding
#else
subroutine test16_rounding
end subroutine test16_rounding
#endif

end program main
