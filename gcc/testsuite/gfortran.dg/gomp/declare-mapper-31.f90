! { dg-do compile }

type t
integer :: x, y
integer, allocatable :: arr(:)
end type t

type(t) :: var

allocate(var%arr(1:20))

var%arr = 0

! If we ask for a named mapper that hasn't been defined, an error should be
! raised.  This isn't a *syntax* error, so the !$omp target..!$omp end target
! block should still be parsed correctly.
!$omp target map(mapper(arraymapper), tofrom: var)
! { dg-error "User-defined mapper .arraymapper. not found" "" { target *-*-* } .-1 }
block
var%arr(5) = 5
end block
!$omp end target

! OTOH, this is a syntax error, and the offload block is not recognized.
!$omp target map(
! { dg-error "Syntax error in OpenMP variable list" "" { target *-*-* } .-1 }
block
var%arr(6) = 6
end block

! ...but not for the specific name 'default'.
!$omp target map(mapper(default), tofrom: var)
block
var%arr(5) = 5
end block
!$omp end target

end
