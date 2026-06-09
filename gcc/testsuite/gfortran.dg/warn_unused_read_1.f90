! { dg-do compile }
! { dg-additional-options "-Wunused-read" }
program main
  real :: x, y
  read (*,*) x ! { dg-warning "read at" }
  read (*,*) y
  print *,y
end program main
