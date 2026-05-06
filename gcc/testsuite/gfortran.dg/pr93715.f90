! { dg-do compile }
! { dg-options "-fcoarray=single" }
!
! PR fortran/93715
! A scalar coarray used in asynchronous I/O caused an ICE in
! gfc_trans_auto_array_allocation.

program p
   integer :: a, b[*]
   read (1, *, asynchronous='yes') a, b
end
