! Check that type renaming via module use is properly handled.

module zero
  implicit none
  type t
    integer, pointer :: x(:)
    integer, pointer :: y(:)
  end type t
end module zero

module first
  use zero, only: t1 => t
  implicit none
  !$omp declare mapper(t1 :: v1) map(v1, v1%x)
end module

module second
  use zero, only: t2 => t
  implicit none
  !$omp declare mapper(t2 :: v2) map(v2, v2%x, v2%y)
end module

subroutine sub2
  use first  ! { dg-note "Previous !.OMP DECLARE MAPPER from module 'first'" }
  use second ! { dg-error "Ambiguous !.OMP DECLARE MAPPER 'default' for type 't' from module 'second'" }
  implicit none
  ! type(t1) :: var1
  ! type(t2) :: var2
end
