! { dg-do compile }
! { dg-additional-options "-fdump-tree-original -fdump-tree-gimple" }

! Check that map clauses are only merged if they have the same modifiers.
! Also check that the GIMPLE pass removes duplicates resulting from early clause
! expansion in the front-end.

subroutine f
  implicit none
  integer :: x, i

  !$omp target map(always,to: x) map(tofrom: x)
    do i = 1, 17
      x = x + i
    end do
  !$omp end target
  !$omp target map(tofrom: x) map(always,from: x)
    do i = 1, 17
      x = x + i
    end do
  !$omp end target
  !$omp target map(to: x) map(present,alloc: x) map(from: x)
    do i = 1, 17
      x = x + i
    end do
  !$omp end target
  !$omp target map(close,always,from: x) map(close,present,alloc: x) map(present,to: x)
    do i = 1, 17
      x = x + i
    end do
  !$omp end target
  !$omp target map(always,to: x) map(always,present,alloc: x) map(present,from: x)
    do i = 1, 17
      x = x + i
    end do
  !$omp end target
end subroutine f

! { dg-final { scan-tree-dump-times {map\(always,to:x\) map\(tofrom:x\)} 1 "original" } }
! { dg-final { scan-tree-dump-times {map\(tofrom:x\) map\(always,from:x\)} 1 "original" } }
! { dg-final { scan-tree-dump-times {map\(present,alloc:x\) map\(tofrom:x\)} 1 "original" } }
! { dg-final { scan-tree-dump-times {map\(always,from:x\) map\(present,to:x\)} 1 "original" } }
! { dg-final { scan-tree-dump-times {map\(always,to:x\) map\(present,from:x\)} 1 "original" } }

subroutine g (a)
  integer, intent(inout) :: a(:)

  !$omp target map(always, to: a) map(tofrom: a)
    a = a * 4
  !$omp end target
  !$omp target map(close, present, tofrom: a) map(always, from: a)
    a = a * 8
  !$omp end target

end subroutine

! { dg-final { scan-tree-dump-times {map\(alloc:a\.0 \[pointer assign, bias: 0\]\)} 4 "original" } }
! { dg-final { scan-tree-dump-times {map\(alloc:a\.0 \[pointer assign, bias: 0\]\)} 2 "gimple" } }


