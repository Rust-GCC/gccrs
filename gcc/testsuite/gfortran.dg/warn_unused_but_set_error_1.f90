! { dg-do compile }
! { dg-options "-Wundefined-vars -Wunused-but-set-variable" }
! Make sure that no warning are emitted after an error.
program main
  real :: a, b
  b = sum(a) ! { dg-error "must be an array" }
end program main
  
