! { dg-do compile }
! PR fortran/88048
program p
   integer, parameter :: a(2) = 1
   data a(2) /a(1)/  ! { dg-error "shall not appear in a DATA statement" }
   print *, a
end
