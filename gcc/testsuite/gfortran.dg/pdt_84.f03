! { dg-do compile }
!
! Test the fix for PR108663
!
! Contributed by
!
module m
   type t(n)
      integer, len :: n
      integer :: a(n)
   end type
contains
   subroutine s(x, arr)
      type(t(2)) :: x
      integer :: arr(2)
      if (any (x%a /= arr)) stop 1
   end
end
program p
   use m, only: t, pdtt, s       ! { dg-error "not found in module" }

   type(t(2)) :: y = t(2)([1,2])
   type (pdtt)  :: z             ! { dg-error "being used before it is defined" }

   call s(y, [1,2])
   y = t(2)([3,4])
   call s(y, [3,4])
end
