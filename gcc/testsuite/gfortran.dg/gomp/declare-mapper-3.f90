! { dg-do compile }

! Simple module test.

module m
 implicit none
 type t
   integer :: x
 end type t
 !$omp declare mapper(t :: v) map(v)
 !$omp declare mapper(only_x : t :: y) map(y%x)

type(t) :: var2
contains
subroutine s
!$omp target enter data map(mapper(default), to: var2)  ! { dg-error "Sorry, declared mapper 'default', used for 'var2' at .1., is not yet supported" }
end
end module m

use m
implicit none
type(t) :: var
!$omp target enter data map(mapper(default), to: var)  ! { dg-error "Sorry, declared mapper 'default', used for 'var' at .1., is not yet supported" }

!$omp target exit data map(mapper(only_x), release: var)  ! { dg-error "Sorry, declared mapper 'only_x', used for 'var' at .1., is not yet supported" }

!$omp target map(var)  ! { dg-error "Sorry, declared mapper 'default', used for 'var' at .1., is not yet supported" }
!$omp end target
end
