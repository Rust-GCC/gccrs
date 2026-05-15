! Test multiple map clauses for the same variable in target enter/exit
! data constructs, including release and delete map-types.
! { dg-do run }

program main
  implicit none
  integer, parameter :: N = 100
  integer :: a(N), s, i

  ! delete + release in exit data.
  a = [(i, i=1,N)]
  !$omp target enter data map(alloc: a) map(to: a)
  s = 0
  !$omp target map(alloc: a) map(tofrom: s)
    do i = 1, N
      s = s + a(i)
    end do
  !$omp end target
  if (s /= N * (N + 1) / 2) stop 1
  !$omp target exit data map(delete: a) map(release: a)

  ! release + release: duplicate release decrements the reference count
  ! once (deduplicated).  Two enter data calls set refcount to 2, so after
  ! one deduplicated release refcount is 1 and the mapping remains.
  a = [(i, i=1,N)]
  !$omp target enter data map(to: a)   ! refcount = 1
  !$omp target enter data map(to: a)   ! refcount = 2
  !$omp target exit data map(release: a) map(release: a)   ! refcount = 1
  s = 0
  !$omp target map(alloc: a) map(tofrom: s)
    do i = 1, N
      s = s + a(i)
    end do
  !$omp end target
  if (s /= N * (N + 1) / 2) stop 2
  !$omp target exit data map(delete: a)   ! refcount = 0

  ! delete + delete: duplicate delete removes the mapping unconditionally
  ! once (deduplicated).
  a = [(i, i=1,N)]
  !$omp target enter data map(to: a)
  !$omp target exit data map(delete: a) map(delete: a)

  ! from + release: copy device values back to host and release the mapping.
  a = [(i, i=1,N)]
  !$omp target enter data map(to: a)   ! refcount = 1
  !$omp target
    a = a * 3
  !$omp end target
  !$omp target exit data map(release: a) map(from: a)   ! refcount = 0
  if (any (a /= [(i*3, i=1,N)])) stop 3
  if (any (a /= [(i*3, i=1,N)])) stop 4

  ! Same tests via a subroutine to verify correct behaviour with
  ! dummy arguments (passed by reference).
  call test_dummy (a)

contains

  subroutine test_dummy (a)
    integer, intent(inout) :: a(:)
    integer :: s, i

    ! delete + release with dummy argument.
    a = [(i, i=1,size(a))]
    !$omp target enter data map(alloc: a) map(to: a)
    s = 0
    !$omp target map(alloc: a) map(tofrom: s)
      do i = 1, size(a)
        s = s + a(i)
      end do
    !$omp end target
    if (s /= size(a) * (size(a) + 1) / 2) stop 5
    !$omp target exit data map(delete: a) map(release: a)

    ! from + release with dummy argument.
    a = [(i, i=1,size(a))]
    !$omp target enter data map(to: a)
    !$omp target
      a = a * 3
    !$omp end target
    !$omp target exit data map(release: a) map(from: a)
    if (any (a /= [(i*3, i=1,size(a))])) stop 6

  end subroutine

end program
