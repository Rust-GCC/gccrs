! { dg-do compile }
! { dg-options "-fopenmp" }
!
! PR fortran/107425
! ICE in gimplify_var_or_parm_decl when an implicitly typed variable
! is used as a subscript in an iterator depend clause.

program p
   integer :: x(8)
   !$omp taskwait depend(iterator(i=1:8), in:x(j))
end
