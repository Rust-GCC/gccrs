! { dg-do run }
! PR93727 Test EX format rounding of truncated hex mantissa.
! Verifies ROUND_NEAREST (round-to-nearest ties-to-even) when the
! user specifies a d less than full precision.
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

  ! Carry propagates through all fractional digits into integer digit;
  ! integer F overflows -> normalize to 8 with exponent incremented.
  ! huge(1.0_4) = 0XF.FFFFFP+124
  ! d=4: first dropped F > 8, all frac digits carry to 0, F+1 -> 8, exp 125
  r4 = huge(1.0_4)
  write(s, '(EX0.4)') r4
  if (s(1:13) /= '0X8.0000P+125') stop 1

end subroutine test04

subroutine test08
  real(kind=8) :: r8
  character(kind=1,len=40) :: s

  ! Round up: first dropped digit B (=11) > 8, with carry through F into D.
  ! -pi*25 = -0X9.D14707B63DFBP+3 (12 significant frac digits, 13th = 0)
  ! d=11 keeps D14707B63DF, first dropped B -> round up
  ! last kept F + 1 = 0 carry, D + 1 = E, result: -0X9.D14707B63E0P+3
  r8 = -3.14159682678_8 * 25._8
  write(s, '(EX0.11)') r8
  if (s(1:19) /= '-0X9.D14707B63E0P+3') stop 2

  ! Tie, last kept digit even (C=12) -> truncate (round to even).
  ! 10.736328125 = 0XA.BC8000000000P+0
  ! d=2: first dropped 8, tail zeros, C even -> truncate -> 0XA.BCP+0
  r8 = 10.736328125_8
  write(s, '(EX0.2)') r8
  if (s(1:9) /= '0XA.BCP+0') stop 3

  ! Tie, last kept digit odd (B=11) -> round up (round to even).
  ! 10.732421875 = 0XA.BB8000000000P+0
  ! d=2: first dropped 8, tail zeros, B odd -> round up, B+1=C -> 0XA.BCP+0
  r8 = 10.732421875_8
  write(s, '(EX0.2)') r8
  if (s(1:9) /= '0XA.BCP+0') stop 4

  ! Round up: first dropped digit 9 > 8, no carry.
  ! 10.736572265625 = 0XA.BC9000000000P+0
  ! d=2: first dropped 9 > 8 -> round up, C+1=D -> 0XA.BDP+0
  r8 = 10.736572265625_8
  write(s, '(EX0.2)') r8
  if (s(1:9) /= '0XA.BDP+0') stop 5

end subroutine test08

#ifdef __GFC_REAL_10__
subroutine test10
  real(kind=10) :: r10
  character(kind=1,len=40) :: s

  ! Tie, last kept digit even (C=12) -> truncate.
  ! 10.736328125 = 0XA.BC8000000000000P+0 (15 frac digits)
  ! d=2: first dropped 8, tail zeros, C even -> truncate -> 0XA.BCP+0
  r10 = 10.736328125_10
  write(s, '(EX0.2)') r10
  if (s(1:9) /= '0XA.BCP+0') stop 6

  ! Tie, last kept digit odd (B=11) -> round up.
  ! 10.732421875 = 0XA.BB8000000000000P+0
  ! d=2: first dropped 8, tail zeros, B odd -> round up -> 0XA.BCP+0
  r10 = 10.732421875_10
  write(s, '(EX0.2)') r10
  if (s(1:9) /= '0XA.BCP+0') stop 7

  ! Round up: first dropped digit 9 > 8, no carry.
  ! 10.736572265625 = 0XA.BC9000000000000P+0
  ! d=2: first dropped 9 > 8 -> round up, C+1=D -> 0XA.BDP+0
  r10 = 10.736572265625_10
  write(s, '(EX0.2)') r10
  if (s(1:9) /= '0XA.BDP+0') stop 8

  ! Round up: first dropped digit 9 > 8 in -pi*25 at d=13.
  ! -pi*25 = -0X9.D14707B63DFB497P+3 (15 frac digits)
  ! d=13: first dropped 9 > 8, last kept 4+1=5, no carry
  ! result: -0X9.D14707B63DFB5P+3
  r10 = -3.14159682678_10 * 25._10
  write(s, '(EX0.13)') r10
  if (s(1:21) /= '-0X9.D14707B63DFB5P+3') stop 9

  ! Normalization: carry through all 14 frac digits into integer.
  ! huge(1.0_10) = 0XF.FFFFFFFFFFFFFFFP+16380 (15 frac digits)
  ! d=14: first dropped F > 8, all carry, F+1 -> 8, exp 16381
  r10 = huge(1.0_10)
  write(s, '(EX0.14)') r10
  if (s(1:25) /= '0X8.00000000000000P+16381') stop 10

end subroutine test10
#else
subroutine test10
end subroutine test10
#endif

#ifdef __GFC_REAL_16__
subroutine test16
  real(kind=16) :: r16
  character(kind=1,len=45) :: s

  ! Tie, last kept digit even (C=12) -> truncate.
  ! 10.736328125 = 0XA.BC8000000000000000000000000P+0 (28 frac digits)
  ! d=2: first dropped 8, tail zeros, C even -> truncate -> 0XA.BCP+0
  r16 = 10.736328125_16
  write(s, '(EX0.2)') r16
  if (s(1:9) /= '0XA.BCP+0') stop 11

  ! Tie, last kept digit odd (B=11) -> round up.
  ! 10.732421875 = 0XA.BB8000000000000000000000000P+0
  ! d=2: first dropped 8, tail zeros, B odd -> round up -> 0XA.BCP+0
  r16 = 10.732421875_16
  write(s, '(EX0.2)') r16
  if (s(1:9) /= '0XA.BCP+0') stop 12

  ! Round up: first dropped digit 9 > 8, no carry.
  ! 10.736572265625 = 0XA.BC9000000000000000000000000P+0
  ! d=2: first dropped 9 > 8 -> round up, C+1=D -> 0XA.BDP+0
  r16 = 10.736572265625_16
  write(s, '(EX0.2)') r16
  if (s(1:9) /= '0XA.BDP+0') stop 13

  ! Round up at d=26 on 1/3 = 0XA.AAAAAAAAAAAAAAAAAAAAAAAAAAA8P-5
  ! d=26: first dropped A (=10) > 8, last kept A+1=B, no carry
  ! result: 0XA.AAAAAAAAAAAAAAAAAAAAAAAAABP-5
  r16 = 1.0_16 / 3.0_16
  write(s, '(EX0.26)') r16
  if (s(1:33) /= '0XA.AAAAAAAAAAAAAAAAAAAAAAAAABP-5') stop 14

  ! Tie and normalization: huge(1.0_16) = 0XF.FFFFFFFFFFFFFFFFFFFFFFFFFFF8P+16380
  ! d=27: first dropped 8, tail empty -> tie, last kept F (odd) -> round up
  ! all 27 F's carry, F+1 -> 8, exp 16381
  ! result: 0X8.000000000000000000000000000P+16381
  r16 = huge(1.0_16)
  write(s, '(EX0.27)') r16
  if (s(1:38) /= '0X8.000000000000000000000000000P+16381') stop 15

end subroutine test16
#else
subroutine test16
end subroutine test16
#endif

end program main
