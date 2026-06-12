! { dg-do compile }

! The following are two declare mapper.
! Technically, they aren't ambiguous as they are identical
! but still we expect an error.

module one
  implicit none
  type t
    integer, pointer :: x(:)
  end type t
end module

module two
  use one
  implicit none
  !$omp declare mapper(t :: v) map(v, v%x)
end module two

module three
  use one
  implicit none
  !$omp declare mapper(t :: v) map(v, v%x)
end module three

subroutine sub1
  use two   ! { dg-note "Previous !.OMP DECLARE MAPPER from module 'two'" }
  use three ! { dg-error "Ambiguous !.OMP DECLARE MAPPER 'default' for type 't' from module 'three' at .1." }
  implicit none
  type(t) :: var

  allocate(var%x(1:20))
  var%x = 0

  !$omp target map(tofrom: var)
  block
    var%x = 1
  end block
end
