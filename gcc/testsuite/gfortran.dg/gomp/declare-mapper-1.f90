! Check that other variables are fine to be mapped - but only if the var itself is mapped

subroutine one
implicit none
type t
  integer :: x(5)
end type

integer :: q, z

!$omp declare mapper(t :: v) map(v%x(1:5)) ! { dg-error "Sorry, 'declare mapper', used at .1., is not yet implemented" }
!$omp declare mapper(my_name : t :: v2) map(q) map(v2) map(z)

type(t) :: var(4)
type(t) :: var2(4)

  !$omp target enter data map(var)
  !$omp target enter data map(mapper(my_name), to : var2)

!$omp assume contains(declare mapper) ! { dg-error "Invalid 'DECLARE MAPPER' directive at .1. in CONTAINS clause: declarative, informational, and meta directives not permitted" }
block
end block
end


subroutine two
implicit none
type t
end type t
integer :: y
!$omp declare mapper( t :: var) map(to: y) ! { dg-error "At least one 'map' clause in !.OMP DECLARE MAPPER at .1. must map 'var' or an element of it" }
! { dg-error "Sorry, 'declare mapper', used at .1., is not yet implemented" "" { target *-*-* } .-1 }
end


subroutine three
implicit none
type t
end type t
integer :: y
!$omp declare mapper( t :: var) ! { dg-error "At least one 'map' clause in !.OMP DECLARE MAPPER at .1. must map 'var' or an element of it" }
! { dg-error "Sorry, 'declare mapper', used at .1., is not yet implemented" "" { target *-*-* } .-1 }
end

subroutine four
  type t
  end type t
  !$omp declare mapper(my_id : t :: v2) map(v2)  ! { dg-note "Previous !.OMP DECLARE MAPPER here" }

  !$omp declare mapper(my_id : t :: v3) map(v3)  ! { dg-error "Redefinition of !.OMP DECLARE MAPPER at .1. for type 'TYPE\\(t\\)' with id 'my_id'" }

  !$omp declare mapper(t :: v4) map(v4)  ! { dg-note "Previous !.OMP DECLARE MAPPER here" }
! { dg-error "Sorry, 'declare mapper', used at .1., is not yet implemented" "" { target *-*-* } .-1 }
  !$omp declare mapper(t :: v5) map(v5)  ! { dg-error "Redefinition of !.OMP DECLARE MAPPER at .1. for type 'TYPE\\(t\\)'" }
end
