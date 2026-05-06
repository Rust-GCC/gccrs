! { dg-additional-options -O3 }
! { dg-additional-options --param=openacc-kernels=decompose }

include '../libgomp.fortran/pr95551-1.f90'
