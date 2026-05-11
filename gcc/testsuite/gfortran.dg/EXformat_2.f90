! { dg-do run }
! PR93727 Test writing EX as character(kind=1) output
program kind1
  implicit none
  integer, parameter :: wp = 8
  real(kind=wp) :: num
  character(kind=1,len=45) :: str1
  
  num = -3.14159682678_wp * 25._wp
  write(str1, '(">",EX30.0,"<")') num
  if (str1.ne.">          -0X9.D14707B63DFBP+3<") stop 1
  write(str1, '(">",EX30.1,"<")') num
  if (str1.ne.">                     -0X9.DP+3<") stop 2
  write(str1, '(">",EX30.2,"<")') num
  if (str1.ne.">                    -0X9.D1P+3<") stop 3
  write(str1, '(">",EX30.3,"<")') num
  if (str1.ne.">                   -0X9.D14P+3<") stop 4
  write(str1, '(">",EX30.4,"<")') num
  if (str1.ne.">                  -0X9.D147P+3<") stop 5
  write(str1, '(">",EX30.15e8,"<")') num
  if (str1.ne.">-0X9.D14707B63DFB000P+00000003<") stop 6
  write(str1, '(">",EX8.5,"<")') num
  if (str1.ne.">********<") stop 7
end program kind1
