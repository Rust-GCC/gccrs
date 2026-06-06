! { dg-do compile }
! { dg-additional-options "-O1" }
!
! PR fortran/125606
!
! Derived from Fujitsu testsuite
! Reported by David Binderman

module m
  implicit none
  type ty
     class(*), allocatable :: c1(:)
  end type ty
  type tt
     class(*), allocatable :: node1(:)
  end type tt
  type,extends(tt)::tte
     class(*), allocatable :: c2e(:)
  end type tte
contains
  subroutine put_addr
    class(*), allocatable :: t(:)
    integer :: unit
    select type (t)
    class is (tt)
       select type (p=>t(2)%node1)
       class is (ty)
          write (unit) loc(p(2)%c1)
       end select
       select type (t)
       type is (tte)
          write (7) loc(t(2)%c2e)
       end select
    end select
  end subroutine put_addr
end module m
