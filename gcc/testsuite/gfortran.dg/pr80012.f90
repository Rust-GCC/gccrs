! { dg-do compile }
! { dg-options "-std=gnu" }
! PR fortran/80012
! Test that the error message for ambiguous pointer function assignment
! is split into an error and an informational note, without FIXME.

two() = 7
contains
  function two () ! { dg-error "INTERNAL-PROC procedure at .1. is already declared as STATEMENT-PROC procedure" }
! { dg-message "F2008: A pointer function assignment is ambiguous" "" { target *-*-* } 9 }
    integer, pointer  :: two
    allocate(two)
  end function two
end
