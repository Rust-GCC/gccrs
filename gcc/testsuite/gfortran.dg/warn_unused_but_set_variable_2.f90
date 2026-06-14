! { dg-do compile }
! { dg-additional-options "-Wunused-but-set-variable" }
program main
  integer :: i,j, k
  real, dimension(100) :: a
  integer, dimension(50) :: ios
  i = 42
  j = 42
  read (*,*) a(i)
  print *, a(j)

  k = 42
  read (*,*,iostat=ios(k)) a(10)
  print *,ios(42)
end program main
  
