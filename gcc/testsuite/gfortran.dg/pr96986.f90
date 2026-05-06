! { dg-do compile }
! { dg-options "-std=legacy" }
!
! PR fortran/96986
! Calling an ENTRY with no volatile arguments incorrectly required an
! explicit interface because the volatile attribute from a sibling ENTRY
! was checked against the master procedure's combined formal list.

subroutine volatile_test ()
  implicit none
  integer, volatile :: va

  entry fun_a()
  return

  entry fun_b(va)
    call fun_c()
  return
end subroutine volatile_test

subroutine fun_c ()
  implicit none
  call fun_a()  ! Must not require explicit interface
  return
end subroutine fun_c
