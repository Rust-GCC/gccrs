! PR fortran/101760
! { dg-do compile }
! { dg-options "-fopenmp -O1" }

! Verify that deferred-length character variables with the TARGET attribute
! in OpenMP target regions do not ICE during inlining.

program p
   character(:), allocatable, target :: x
   logical :: l
   !$omp target map(from: l)
   l = allocated (x)
   !$omp end target
end
