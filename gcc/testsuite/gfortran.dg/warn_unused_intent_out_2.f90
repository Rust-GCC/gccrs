! { dg-do compile }
! { dg-options "-Wunused-intent-out" }
! PR fortran/125801 - no warnings should be emitted here.

subroutine unused_1
  character(len=20) c_val
  integer val

  CALL getarg(1,c_val)
  READ(c_val,*) val
  print*,val
end subroutine unused_1

subroutine unused_2

  INTEGER t1,t2
  CALL SYSTEM_CLOCK(t1)
  CALL sleep(1)
  CALL SYSTEM_CLOCK(t2)
  print*,"Ende: ",(t2-t1)/1000.0
end subroutine unused_2
