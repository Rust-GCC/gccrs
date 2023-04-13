! { dg-additional-sources my-usleep.c }
! { dg-additional-options -Wno-complain-wrong-lang }

program main
  implicit none
  interface
    subroutine usleep(t) bind(C, name="my_usleep")
      use iso_c_binding
      integer(c_int), value :: t
    end subroutine
  end interface

  integer :: a(128)
  integer :: i

  !$omp parallel num_threads(8)
    !$omp barrier
    !$omp do schedule (dynamic, 2) order(reproducible:concurrent)
    do i = 1, 128
      a(i) = i
      if (i == 1) then
        call usleep (20)
      else if (i == 18) then
        call usleep (40)
      end if
    end do
    !$omp end do nowait
    !$omp do schedule (dynamic, 2) order(reproducible:concurrent)
    do i = 1, 128
      a(i) = a(i) + i
    end do
    !$omp end do nowait
  !$omp end parallel
  do i = 1, 128
    if (a(i) /= 2 * i) &
      stop
  end do
end program main
