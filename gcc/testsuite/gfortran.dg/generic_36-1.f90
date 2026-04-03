! { dg-do compile }
!
! PR fortran/66973 - resolution of generic interface with TYPE(C_PTR)

MODULE H5T
  USE ISO_C_BINDING, only: C_LOC,    C_PTR,    C_NULL_PTR, &
                           C_FUNLOC, C_FUNPTR, C_NULL_FUNPTR
  IMPLICIT NONE
  public :: pickone, cnt, test1
  public :: C_LOC, C_PTR, C_NULL_PTR, C_FUNLOC, C_FUNPTR, C_NULL_FUNPTR
  private

  INTERFACE pickone
     MODULE PROCEDURE pick_f03    ! order matters for the test!
     MODULE PROCEDURE pick_funptr
     MODULE PROCEDURE pick_f90
  END INTERFACE

  integer :: cnt(3) = 0

CONTAINS

  SUBROUTINE pick_f90(int_value)
    IMPLICIT NONE
    INTEGER, INTENT(IN) :: int_value
    PRINT*,'Inside pick_f90'
    cnt(1) = cnt(1) + 1
  END SUBROUTINE pick_f90

  SUBROUTINE pick_f03(value)
    IMPLICIT NONE
    TYPE(C_PTR), INTENT(IN) :: value
    PRINT*,'Inside pick_f03'
    cnt(2) = cnt(2) + 1
  END SUBROUTINE pick_f03

  SUBROUTINE pick_funptr(addr)
    IMPLICIT NONE
    TYPE(C_FUNPTR), INTENT(IN) :: addr
    PRINT*,'Inside pick_funptr'
    cnt(3) = cnt(3) + 1
  END SUBROUTINE pick_funptr

  subroutine test1 ()
    integer      :: intval
    REAL, TARGET :: val
    type(c_ptr)  :: ptr
    type(c_funptr) :: funptr
    procedure(), pointer :: indirect => null()
    cnt = 0
    CALL pickone(intval)
!   print *, cnt
    if (any (cnt /= [1,0,0])) stop 1
    cnt = 0
    CALL pickone(ptr)
    CALL pickone(c_null_ptr)
    CALL pickone(C_LOC(val))
!   print *, cnt
    if (any (cnt /= [0,3,0])) stop 2
    cnt = 0
    CALL pickone(funptr)
    CALL pickone(c_null_funptr)
    CALL pickone(C_FUNLOC(indirect))
!   print *, cnt
    if (any (cnt /= [0,0,3])) stop 3
  end subroutine test1

END MODULE H5T
