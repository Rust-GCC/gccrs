! Auxiliary file for submodule_host_assoc_1.f90
! { dg-skip-if "" { *-*-* } }
submodule(m) s
  implicit none
contains
  module subroutine test()
    if (abs(double_it(3.0d0) - 6.0d0) > 1d-10) stop 1
  end subroutine
end submodule s

program p
  use m
  implicit none
  call test()
end program p
