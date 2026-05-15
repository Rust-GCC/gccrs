! Test multiple map clauses for the same variable with various
! combinations of map-types: alloc, to, from, tofrom.
! { dg-do run }

program main
  implicit none
  integer, parameter :: N = 100
  integer :: a(N), s, i

  ! map(to) + map(alloc) + map(from) = map(tofrom).
  a = [(i, i=1,N)]
  !$omp target map(to: a) map(alloc: a) map(from: a)
    a = a * 4
  !$omp end target
  if (any (a /= [(i*4, i=1,N)])) stop 1

  ! map(to) + map(tofrom) + map(to): tofrom covers both directions.
  a = [(i, i=1,N)]
  !$omp target map(to: a) map(tofrom: a) map(to: a)
    a = a * 5
  !$omp end target
  if (any (a /= [(i*5, i=1,N)])) stop 2

  ! map(from) + map(tofrom) + map(from): tofrom covers both directions.
  a = [(i, i=1,N)]
  !$omp target map(from: a) map(tofrom: a) map(from: a)
    a = a * 6
  !$omp end target
  if (any (a /= [(i*6, i=1,N)])) stop 3

  ! map(alloc) + map(to): device gets host values via 'to'.
  ! Also tests map(tofrom) + map(alloc) on the scalar s.
  a = [(i, i=1,N)]
  s = 0
  !$omp target map(alloc: a) map(to: a) map(tofrom: s) map(alloc: s)
    do i = 1, N
      s = s + a(i)
    end do
  !$omp end target
  if (s /= N * (N + 1) / 2) stop 4

  ! map(alloc) + map(from) + map(alloc): device values come back via 'from'.
  !$omp target map(alloc: a) map(from: a) map(alloc: a)
    do i = 1, N
      a(i) = i * 7
    end do
  !$omp end target
  if (any (a /= [(i*7, i=1,N)])) stop 5

  ! map(alloc) + map(tofrom) + map(alloc): full bidirectional transfer.
  a = [(i, i=1,N)]
  !$omp target map(alloc: a) map(tofrom: a) map(alloc: a)
    a = a * 8
  !$omp end target
  if (any (a /= [(i*8, i=1,N)])) stop 6

  ! Same tests via a subroutine to verify correct behaviour with
  ! dummy arguments (passed by reference).
  call test_dummy (a)

contains

  subroutine test_dummy (a)
    integer, intent(inout) :: a(:)
    integer :: i

    ! map(to) + map(alloc) + map(from) with dummy argument.
    a = [(i, i=1,size(a))]
    !$omp target map(to: a) map(alloc: a) map(from: a)
      a = a * 4
    !$omp end target
    if (any (a /= [(i*4, i=1,size(a))])) stop 7

    ! map(alloc) + map(tofrom) + map(alloc) with dummy argument.
    a = [(i, i=1,size(a))]
    !$omp target map(alloc: a) map(tofrom: a) map(alloc: a)
      a = a * 8
    !$omp end target
    if (any (a /= [(i*8, i=1,size(a))])) stop 8

  end subroutine

end program
