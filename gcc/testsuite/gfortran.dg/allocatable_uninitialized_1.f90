! { dg-do compile }
! { dg-options "-O -Wall -Wno-unused-but-set-variable -Wno-undefined-vars" }
program main
  real,allocatable:: a(:),b(:)

   a(1)=2*b(1) ! { dg-warning "uninitialized" }
end
