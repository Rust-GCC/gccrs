! Test multiple map clauses for the same variable with the 'always'
! map-type modifier.
! { dg-do run { target offload_device_nonshared_as } }

program main
  implicit none
  integer, parameter :: N = 100
  integer :: a(N), i

  a = [(i, i=1,N)]
  !$omp target data map(tofrom: a)
    ! Device has a(i) = i.  Update the host copy to create divergence.
    a = [(i*2, i=1,N)]

    ! map(always, to) + map(tofrom): 'always' forces the updated host
    ! values (i*2) onto the device despite the existing mapping.
    !$omp target map(always, to: a) map(tofrom: a)
      do i = 1, N
        if (a(i) /= i*2) stop 1
      end do
      a = [(i*3, i=1,N)]
    !$omp end target

    do i = 1, N
      if (a(i) /= i*2) stop 2
    end do  

    ! Reset host for the next test; device retains i*3.
    a = [(i, i=1,N)]

    ! map(tofrom) + map(always, from): 'always' forces new device values
    ! back to host.
    !$omp target map(tofrom: a) map(always, from: a)
      do i = 1, N
        if (a(i) /= i*3) stop 3
        a(i) = i*4
      end do
    !$omp end target
    do i = 1, N
      if (a(i) /= i*4) stop 4
    end do
  !$omp end target data

  ! Same tests via a subroutine to verify correct behaviour with
  ! dummy arguments (passed by reference).
  call test_dummy (a)

contains

  subroutine test_dummy (a)
    integer, intent(inout) :: a(:)
    integer :: i

    a = [(i, i=1,size(a))]
    !$omp target data map(tofrom: a)
      a = [(i*2, i=1,size(a))]

      !$omp target map(always, to: a) map(tofrom: a)
        do i = 1, size(a)
          if (a(i) /= i*2) stop 5
        end do
        a = [(i*3, i=1,size(a))]
      !$omp end target

      do i = 1, size(a)
        if (a(i) /= i*2) stop 6
      end do

      a = [(i, i=1,size(a))]

      !$omp target map(tofrom: a) map(always, from: a)
        do i = 1, size(a)
          if (a(i) /= i*3) stop 7
          a(i) = i*4
        end do
      !$omp end target
      do i = 1, size(a)
        if (a(i) /= i*4) stop 8
      end do
    !$omp end target data

  end subroutine

end program
