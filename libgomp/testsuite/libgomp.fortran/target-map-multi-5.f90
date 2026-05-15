! Test multiple map clauses for the same variable using Fortran array section
! notation (subscript triplets): fixed-size, allocatable, and dummy arrays.
! { dg-do run }

program main
  implicit none
  integer, parameter :: N = 100
  integer :: a(N), s, i
  integer, allocatable :: b(:)

  ! --- Fixed-size array ---

  ! map(to) + map(alloc) + map(from) on the same section.
  a = [(i, i=1,N)]
  !$omp target map(to: a(1:N)) map(alloc: a(1:N)) map(from: a(1:N))
    a = a * 4
  !$omp end target
  if (any (a /= [(i*4, i=1,N)])) stop 1

  ! map(to) + map(tofrom) on the same section.
  a = [(i, i=1,N)]
  !$omp target map(to: a(1:N)) map(tofrom: a(1:N))
    a = a * 5
  !$omp end target
  if (any (a /= [(i*5, i=1,N)])) stop 2

  ! map(alloc) + map(to): device gets host values via 'to'.
  a = [(i, i=1,N)]
  s = 0
  !$omp target map(alloc: a(1:N)) map(to: a(1:N)) map(tofrom: s)
    do i = 1, N
      s = s + a(i)
    end do
  !$omp end target
  if (s /= N*(N+1)/2) stop 3

  ! map(alloc) + map(from): device values come back via 'from'.
  !$omp target map(alloc: a(1:N)) map(from: a(1:N))
    do i = 1, N
      a(i) = i * 7
    end do
  !$omp end target
  if (any (a /= [(i*7, i=1,N)])) stop 4

  ! map(alloc) + map(tofrom) + map(alloc): full bidirectional.
  a = [(i, i=1,N)]
  !$omp target map(alloc: a(1:N)) map(tofrom: a(1:N)) map(alloc: a(1:N))
    a = a * 8
  !$omp end target
  if (any (a /= [(i*8, i=1,N)])) stop 5

  ! --- Allocatable array ---

  allocate (b(N))

  ! map(to) + map(alloc) + map(from) on the same section.
  b = [(i, i=1,N)]
  !$omp target map(to: b(1:N)) map(alloc: b(1:N)) map(from: b(1:N))
    b = b * 4
  !$omp end target
  if (any (b /= [(i*4, i=1,N)])) stop 6

  ! map(to) + map(tofrom) on the same section.
  b = [(i, i=1,N)]
  !$omp target map(to: b(1:N)) map(tofrom: b(1:N))
    b = b * 5
  !$omp end target
  if (any (b /= [(i*5, i=1,N)])) stop 7

  ! map(alloc) + map(to): device gets host values via 'to'.
  b = [(i, i=1,N)]
  s = 0
  !$omp target map(alloc: b(1:N)) map(to: b(1:N)) map(tofrom: s)
    do i = 1, N
      s = s + b(i)
    end do
  !$omp end target
  if (s /= N*(N+1)/2) stop 8

  ! map(alloc) + map(from): device values come back via 'from'.
  !$omp target map(alloc: b(1:N)) map(from: b(1:N))
    do i = 1, N
      b(i) = i * 7
    end do
  !$omp end target
  if (any (b /= [(i*7, i=1,N)])) stop 9

  ! map(alloc) + map(tofrom) + map(alloc): full bidirectional.
  b = [(i, i=1,N)]
  !$omp target map(alloc: b(1:N)) map(tofrom: b(1:N)) map(alloc: b(1:N))
    b = b * 8
  !$omp end target
  if (any (b /= [(i*8, i=1,N)])) stop 10

  deallocate (b)

  ! Same tests via a subroutine to verify correct behaviour with
  ! dummy arguments (passed by reference / array descriptor).
  call test_dummy (a)

contains

  subroutine test_dummy (a)
    integer, intent(inout) :: a(:)
    integer :: i, s, n

    n = size(a)

    ! map(to) + map(alloc) + map(from) on the same section.
    a = [(i, i=1,n)]
    !$omp target map(to: a(1:n)) map(alloc: a(1:n)) map(from: a(1:n))
      a = a * 4
    !$omp end target
    if (any (a /= [(i*4, i=1,n)])) stop 11

    ! map(to) + map(tofrom) on the same section.
    a = [(i, i=1,n)]
    !$omp target map(to: a(1:n)) map(tofrom: a(1:n))
      a = a * 5
    !$omp end target
    if (any (a /= [(i*5, i=1,n)])) stop 12

    ! map(alloc) + map(to): device gets host values via 'to'.
    a = [(i, i=1,n)]
    s = 0
    !$omp target map(alloc: a(1:n)) map(to: a(1:n)) map(tofrom: s)
      do i = 1, n
        s = s + a(i)
      end do
    !$omp end target
    if (s /= n*(n+1)/2) stop 13

    ! map(alloc) + map(tofrom) + map(alloc): full bidirectional.
    a = [(i, i=1,n)]
    !$omp target map(alloc: a(1:n)) map(tofrom: a(1:n)) map(alloc: a(1:n))
      a = a * 8
    !$omp end target
    if (any (a /= [(i*8, i=1,n)])) stop 14

  end subroutine

end program
