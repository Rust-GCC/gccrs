! { dg-do compile }
! { dg-additional-options "-O1 -fdefault-integer-8" }
! PR fortran/84779
! Contributed by Dominique d'Humieres <dominiq@lps.ens.fr>
!
! ICE with -O1 -fdefault-integer-8 on mixed ENTRY with complex/logical results.

      complex function f2 (a)
      implicit none
      integer   a
      logical   e2
      entry e2 (a)
      if (a > 0) then
        e2 = .true.
      else
        f2 = 45
      endif
      end
