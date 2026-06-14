! { dg-do compile }
! { dg-additional-options "-Wunused-but-set-variable" }
! Some checks of things that should not warn
module mymod
  implicit none
  real :: x
contains
  subroutine print_x
    print *,x
  end subroutine print_x
  subroutine vol
  end subroutine vol
end module mymod

program memain
  use mymod
  implicit none
  integer, volatile :: a
  real, asynchronous :: b
  x = 42.
  call print_x
  a = 43.
  b = 44.
end program memain
