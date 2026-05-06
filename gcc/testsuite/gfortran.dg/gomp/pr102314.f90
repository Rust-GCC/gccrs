! PR fortran/102314
! { dg-do compile }
! { dg-options "-fopenmp -O2" }

! Verify that deferred-length character allocatables used inside
! OpenMP target regions do not ICE during SSA verification or inlining.

program p
   character(:), allocatable :: y
   call s(y)
   !$omp target
   y = 'abc'
   !$omp end target
contains
   subroutine s(x)
      character(:), allocatable :: x
      x = '123'
   end
end
