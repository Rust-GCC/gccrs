module m
  implicit none
  private
  type t
    integer :: x, z
  end type t
  type t2
 !  private
    type(t) :: y
  end type t2
  public :: t2
  !$omp declare mapper(t :: v) map(v%x)
end module m

use m
implicit none
type(t2) :: var
! Shall honor mapper
!$omp target enter data map(var%y) ! { dg-error "Sorry, declared mapper 'default', used for 'var' at .1., is not yet supported" }
end
