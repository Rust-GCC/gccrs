! Auxiliary file for submodule_private_host.f90
! { dg-skip-if "" { *-*-* } }
submodule(m) ms
  implicit none
contains
  module procedure pub
    x = priv(6)
  end procedure
end submodule

program p
  use m, only : pub
  implicit none
  double precision x
  call pub(x)
  if (abs(x - 3.0d0) > 1d-10) stop 1
end program
