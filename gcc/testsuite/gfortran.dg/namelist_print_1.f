! Test Non standard PRINT namelist - PR21432
!
! Contributor Paul Thomas  <pault@gcc.gnu.org>
!
! { dg-do run }
! { dg-options "-std=gnu" }

      real  x
      namelist /mynml/ x
      x = 1
! { dg-output "^" }
      print mynml ! { dg-output "&MYNML(\r*\n+) X=  1.00000000    ,(\r*\n+) /(\r*\n+)" }
      end
