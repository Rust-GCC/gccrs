implicit none
type t
end type t
integer :: a,b,c

!$omp declare mapper           ! { dg-error "Expected '\\('" }
!$omp declare mapper(          ! { dg-error "Expected either a type name at .1. or a map-type identifier, a colon, or a type name" }
!$omp declare mapper(a : b     ! { dg-error "Expected either a type name at .1. or a map-type identifier, a colon, or a type name" }

!$omp declare mapper(t : a     ! { dg-error "Expected either a type name at .1. or a map-type identifier, a colon, or a type name" }
!$omp declare mapper(t :: a    ! { dg-error "Expected '\\)'" }

!$omp declare mapper( name : t ::            ! { dg-error "Expected variable name" }

!$omp declare mapper( name : t :: var        ! { dg-error "Expected '\\)'" }

!$omp declare mapper( name : t :: var) foo   ! { dg-error "Failed to match clause" }


!$omp declare mapper( name : t2 :: var)      ! { dg-error "Expected either a type name at .1. or a map-type identifier, a colon, or a type name" }
!$omp declare mapper( name : integer :: var) ! { dg-error "!.OMP DECLARE MAPPER with non-derived type" }
end
