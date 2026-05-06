! { dg-do compile }
!
! PR fortran/124567 - rank/shape check in interface checking
!
! Variation of gfortran.dg/proc_decl_26.f90

program test
  implicit none

  interface
    subroutine one(a)
      integer a(:)
    end subroutine
    subroutine two(a)
      integer a(..)
    end subroutine
  end interface

  ! Assumed-shape vs. deferred
  call foo(two)  ! { dg-error "Rank mismatch in argument 'a' \\(1/-1\\)" }
  call bar(two)  ! { dg-error "Rank mismatch in argument 'a' \\(1/-1\\)" }

  ! Reversed
  call bas(one)  ! { dg-error "Rank mismatch in argument 'a' \\(-1/1\\)" }
  call bla(one)  ! { dg-error "Rank mismatch in argument 'a' \\(-1/1\\)" }

contains

  subroutine foo(f1)
    procedure(one) :: f1
  end subroutine foo

  subroutine bar(f2)
    interface
      subroutine f2(a)
        integer a(:)
      end subroutine
    end interface
  end subroutine bar

  subroutine bas(f1)
    procedure(two) :: f1
  end subroutine bas

  subroutine bla(f2)
    interface
      subroutine f2(a)
        integer a(..)
      end subroutine
    end interface
  end subroutine bla

end program 
