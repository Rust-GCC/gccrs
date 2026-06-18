! { dg-do compile }
! { dg-additional-options "-fdump-tree-omplower" }

! A lastprivate allocatable (without a corresponding firstprivate) needs an
! outer-variable reference (gfc_omp_private_outer_ref is true), which makes
! lower_rec_input_clauses emit the implicit barrier.  The barrier kind
! depends on the construct the clause ends up on:
!   - combined "parallel do"     -> GOMP_BARRIER_IMPLICIT_PARALLEL  (0)
!   - "do" enclosed in a "parallel" -> GOMP_BARRIER_IMPLICIT_WORKSHARE (1)

subroutine parallel_case
  integer, allocatable :: a(:)
  integer :: i
  !$omp parallel do lastprivate(a)
  do i = 1, 1
     a = [i]
  end do
end subroutine parallel_case

subroutine workshare_case
  integer, allocatable :: a(:)
  integer :: i
  !$omp parallel
  !$omp do lastprivate(a)
  do i = 1, 1
     a = [i]
  end do
  !$omp end do
  !$omp end parallel
end subroutine workshare_case

! { dg-final { scan-tree-dump-times "GOMP_barrier_ext \\(0\\)" 1 "omplower" } }
! { dg-final { scan-tree-dump-times "GOMP_barrier_ext \\(1\\)" 1 "omplower" } }
