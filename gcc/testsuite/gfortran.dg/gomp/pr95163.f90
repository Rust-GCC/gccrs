! { dg-do compile }
! PR fortran/95163
! Contributed by John Donners <john.donners@eviden.com>
!
! ICE in gimplify_adjust_omp_clauses with map and firstprivate on same
! variable.

  integer :: i
  i = 1
!$omp target map(tofrom: i) firstprivate (i)  ! { dg-error "present on both data and map clauses" }
  i = i + 1
!$omp end target
end
