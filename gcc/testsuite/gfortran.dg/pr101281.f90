! { dg-do compile }
! { dg-additional-options "-std=f2008" }
! PR fortran/101281
! Contributed by Tobias Burnus <burnus@gcc.gnu.org>
!
! ICE with -std=f2008 and assumed-rank array.
!
subroutine s(x)
  integer :: x(..)  ! { dg-error "Fortran 2018: Assumed-rank array" }
end
