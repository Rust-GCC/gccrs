! { dg-do compile }
! { dg-additional-options "-fdump-tree-original" }

program p
   integer, allocatable :: a
   !$omp target map(tofrom: a, a)
   !$omp end target
end

! { dg-final { scan-tree-dump-times {map\(tofrom:\*a\)} 1 "original" } }
