! { dg-options "-fopenacc -fdump-tree-omplower" }

module consts
  integer, parameter :: n = 100
end module consts

program main
  use consts
  implicit none

  integer :: i, j
  real ::  a(n) = 0, b(n) = 0, c, d
  real ::  x(n) = 0, y(n), z
  common /BLOCK/ a, b, c, j, d
  common /KERNELS_BLOCK/ x, y, z

  c = 1.0
  !$acc parallel loop copy(/BLOCK/)
  do i = 1, n
     a(i) = b(i) + c
  end do
  !$acc kernels
  do i = 1, n
     x(i) = y(i) + c
  end do
  !$acc end kernels
end program main

! { dg-final { scan-tree-dump-times "omp target oacc_parallel .*map\\(tofrom:a \\\[len: 400\\\]\\)" 1 "omplower" } }
! { dg-final { scan-tree-dump-times "omp target oacc_parallel .*map\\(tofrom:b \\\[len: 400\\\]\\\)" 1 "omplower" } }
! { dg-final { scan-tree-dump-times "omp target oacc_parallel .*map\\(tofrom:c \\\[len: 4\\\]\\)" 1 "omplower" } }

! { dg-final { scan-tree-dump-times "omp target oacc_kernels .*map\\(force_tofrom:i \\\[len: 4\\\]\\)" 1 "omplower" } }
! { dg-final { scan-tree-dump-times "omp target oacc_kernels .*map\\(tofrom:x \\\[len: 400\\\]\\)" 1 "omplower" } }
! { dg-final { scan-tree-dump-times "omp target oacc_kernels .*map\\(tofrom:y \\\[len: 400\\\]\\\)" 1 "omplower" } }
! { dg-final { scan-tree-dump-times "omp target oacc_kernels .*map\\(force_tofrom:c \\\[len: 4\\\]\\)" 1 "omplower" } }

! { dg-final { scan-tree-dump-not "map\\(.*:block\\)" "omplower" } }
! { dg-final { scan-tree-dump-not "map\\(.*:kernels_block\\)" "omplower" } }
