! { dg-do run }
! pr93727 EX Format Specifiers, testing various kinds, default field widths
program main
  implicit none
  character(kind=1, len=48) :: s1

   call test04
   call test08
   call test10
   call test16

contains

subroutine test04
  real(4) :: r4
  r4 = -huge(1.0_4)
  write(s1,"(EX0.0,'<')") r4
  if (s1.ne."-0XF.FFFFFP+124<") stop 1
  write(s1,"(EX0.0,'<')") 1.0_4/r4
  if (s1.ne."-0X8.P-131<") stop 2
end subroutine test04

subroutine test08
  real(8) :: r8
  r8 = -huge( 1.0_8)
  write(s1,"(EX0.0,'<')") r8
  if (s1.ne."-0XF.FFFFFFFFFFFF8P+1020<") stop 3
  write(s1,"(EX0.0,'<')") 1.0_8/r8
  if (s1.ne."-0X8.P-1027<") stop 4
end subroutine test08

#ifdef __GFC_REAL_10__
subroutine test10
  real(10) :: r10
  r10 = -huge(1.0_10)
  write(s1,"(EX0.0,'<')") r10
  if (s1.ne."-0XF.FFFFFFFFFFFFFFFP+16380<") stop 5
  write(s1,"(EX0.0,'<')") 1.0_10/r10
  if (s1.ne."-0X8.P-16387<") stop 6
end subroutine test10
#else
subroutine test10
end subroutine test10
#endif

#ifdef __GFC_REAL_16__
subroutine test16
  real(16) :: r16
  r16 = 1.0_16/3.0_16
  write(s1,"(EX0.0,'<')") r16
  if (s1.ne."0XA.AAAAAAAAAAAAAAAAAAAAAAAAAAA8P-5<") stop 7
end subroutine test16
#else
subroutine test16
end subroutine test16
#endif

end program main
