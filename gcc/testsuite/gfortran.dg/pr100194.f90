! { dg-do compile }
!
! PR fortran/100194
! ICE in gfc_trans_create_temp_array when passing a non-contiguous
! assumed-rank array to a contiguous assumed-rank dummy argument.
!
! Contributed by Martin Diehl <m.diehl@kit.edu>

subroutine s(x)
   real :: x(..)
   call t(x)
contains
   subroutine t(y)
      real, contiguous :: y(..)
   end
end

! The following from the PR, these compiled OK before the patch.
!
! Contributed by G. Steinmetz <gscfq@t-online.de>

subroutine z3(x)
   real, contiguous :: x(..)
   call t(x)
contains
   subroutine t(y)
      real, contiguous :: y(..)
   end
end

subroutine z2(x)
   real, contiguous :: x(..)
   call t(x)
contains
   subroutine t(y)
      real :: y(..)
   end
end


