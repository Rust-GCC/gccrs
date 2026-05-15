! { dg-do compile }
! { dg-additional-options "-fdump-tree-original" }

! Check that extraneous clauses mapping the same variable multiple times are
! either removed or merged.

subroutine f
  implicit none
  integer :: x, i

  !$omp target map(alloc: x) map(to: x) map(alloc: x) map(from: x) map(alloc: x)
    do i = 1, 17
      x = x + i
    end do
  !$omp end target
  !$omp target map(alloc: x) map(alloc: x) map(alloc: x)
    do i = 1, 17
      x = x + i
    end do
  !$omp end target
  !$omp target map(alloc: x) map(to: x) map(alloc: x) map(to: x) map(alloc: x)
    do i = 1, 17
      x = x + i
    end do
  !$omp end target
  !$omp target map(alloc: x) map(from: x) map(alloc: x) map(from: x) map(alloc: x)
    do i = 1, 17
      x = x + i
    end do
  !$omp end target
end subroutine f

! { dg-final { scan-tree-dump-times {map\(tofrom:x\)} 1 "original" } }
! { dg-final { scan-tree-dump-times {map\(alloc:x\)} 1 "original" } }
! { dg-final { scan-tree-dump-times {map\(to:x\)} 1 "original" } }
! { dg-final { scan-tree-dump-times {map\(from:x\)} 1 "original" } }

subroutine g (a)
  integer, intent(inout) :: a(:)

  !$omp target map(to: a) map(alloc: a) map(from: a)
    a = a * 4
  !$omp end target
  !$omp target map(alloc: a) map(tofrom: a) map(alloc: a)
    a = a * 8
  !$omp end target

end subroutine

! { dg-final { scan-tree-dump-times {map\(tofrom:\*a\.0\) map\(alloc:a\.0 \[pointer assign, bias: 0\]\)} 2 "original" } }
