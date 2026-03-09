! { dg-do compile }
!
! Verify that large KIND values create distinct PDT instances.
!
module m
  implicit none
  integer(8), parameter :: k1 = 2147483648_8 + 100_8
  integer(8), parameter :: k2 = k1 + 4294967296_8

  type t(k)
    integer(8), kind :: k
    integer :: v
  end type

  interface chk
    module procedure chk1, chk2
  end interface
contains
  subroutine chk1(x)
    type(t(k1)), intent(in) :: x
  end subroutine

  subroutine chk2(x)
    type(t(k2)), intent(in) :: x
  end subroutine
end module

program p
  use m
  implicit none
  type(t(k1)) :: a
  type(t(k2)) :: b
  call chk(a)
  call chk(b)
end program
