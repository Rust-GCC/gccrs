!
! Eventually: Check that strided use of 'declare mapper' is rejected for
! 'target enter data' but accepted for 'target update'
!
! However, it currently fails already for parsing, but it should at least not ICE
!
module m
implicit none
type t
  integer :: n
  integer,pointer :: p(:)
end type t

!$omp declare mapper(my : t :: s) map(to: s, s%p(:s%n:2))
contains

subroutine f()
  type(t) v
  !$omp target enter data map(mapper(my), to: v) ! FIXME/TODO: This line should get rejected
  !$omp target update to(mapper(my): v)  ! { dg-error "Syntax error in OpenMP variable list" }
end
end
