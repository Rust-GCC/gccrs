! { dg-do compile }

program asyncwait
  integer, parameter :: N = 64
  real, allocatable :: a(:), b(:)
  integer i

  allocate (a(N))
  allocate (b(N))

  a(:) = 3.0
  b(:) = 0.0

  !$acc wait async (1 2) ! { dg-error "Failed to match clause" }

  !$acc wait async (1,) ! { dg-error "Failed to match clause" }

  !$acc wait async (,1) ! { dg-error "Invalid character in name" }

  !$acc wait async (1, 2, ) ! { dg-error "Failed to match clause" }

  !$acc wait async (1, 2, ,) ! { dg-error "Failed to match clause" }

  !$acc wait async (1 ! { dg-error "Failed to match clause" }

  !$acc wait async (1, *) ! { dg-error "Failed to match clause" }

  !$acc wait async (1, a) ! { dg-error "Failed to match clause" }

  !$acc wait async (a) ! { dg-error "ASYNC clause at \\\(1\\\) requires a scalar INTEGER expression" }

  !$acc wait async (N)

  !$acc wait async (1.0) ! { dg-error "ASYNC clause at \\\(1\\\) requires a scalar INTEGER expression" }

  !$acc wait async 1 ! { dg-error "Failed to match clause" }

  !$acc waitasync ! { dg-error "Failed to match clause" }

  !$acc wait,async ! { dg-error "Failed to match clause" }
end program asyncwait
