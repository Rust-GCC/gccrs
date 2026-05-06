! { dg-do compile }
! { dg-set-target-env-var MALLOC_PERTURB_ "165" }
! PR fortran/79524
! Reject parameter arrays with variable-length CHARACTER elements without
! leaving the temporary charlen from the failed declaration on cl_list.
program p
  character(*), parameter :: z(2) = [character(n) :: 'x', 'y'] ! { dg-error "Cannot initialize parameter array" }
  character(*), parameter :: w(2) = [character(n+1) :: 'a', 'b'] ! { dg-error "Cannot initialize parameter array" }
end
