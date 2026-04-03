! { dg-do run }
! { dg-compile-aux-modules "generic_36-1.f90" }
! { dg-additional-sources generic_36-1.f90 }
! { dg-additional-options "-O2" }
!
! PR fortran/66973 - resolution of generic interface with TYPE(C_PTR)

PROGRAM main
  USE H5T, only: pickone, cnt, test1,            &
                 c_loc, c_ptr, c_null_ptr,       &
                 c_funloc, c_funptr, c_null_funptr
  IMPLICIT NONE
  integer      :: intval
  REAL, TARGET :: val
  type(c_ptr)  :: ptr
  type(c_funptr) :: funptr
  procedure(), pointer :: indirect => null()
  cnt = 0
  call test1
  cnt = 0
  CALL pickone(intval)
! print *, cnt
  if (any (cnt /= [1,0,0])) stop 11
  cnt = 0
  CALL pickone(ptr)
  CALL pickone(c_null_ptr)
  CALL pickone(C_LOC(val))
! print *, cnt
  if (any (cnt /= [0,3,0])) stop 12
  cnt = 0
  CALL pickone(funptr)
  CALL pickone(c_null_funptr)
  CALL pickone(C_FUNLOC(indirect))
! print *, cnt
  if (any (cnt /= [0,0,3])) stop 13
END PROGRAM main
