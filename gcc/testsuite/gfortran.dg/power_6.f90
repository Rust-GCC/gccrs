! { dg-do compile }
! { dg-options "-O1 -fdump-tree-optimized" }
!
! PR middle-end/57073
! See also PR 57073
!
real function f(k)
  integer, value :: k
  f = (-1.0)**k
end

! { dg-final { scan-tree-dump-not "__builtin_powif"  "optimized" } }
