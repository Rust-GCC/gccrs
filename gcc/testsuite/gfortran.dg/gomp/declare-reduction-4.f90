! Check that type renaming via module use is properly handled.

module zero
  implicit none
  type t
    integer :: x = 0
  end type t
end module zero

module first
  use zero, only: t1 => t
  implicit none
  !$omp declare reduction(+: t1 : omp_out%x = omp_out%x + omp_in%x)
end module

module second
  use zero, only: t2 => t
  implicit none
  !$omp declare reduction(+: t2 : omp_out%x = omp_out%x + omp_in%x)
end module

subroutine sub2
  use first  ! { dg-note "Previous !.OMP DECLARE REDUCTION from module 'first'" }
  use second ! { dg-error "Ambiguous !.OMP DECLARE REDUCTION 'operator \\+' for type 'TYPE\\(t\\)' from module 'second' at .1." }
  implicit none
  type(t1) :: var(5), sum1
  type(t2) :: var2(3), sum2
end
