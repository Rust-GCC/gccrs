! Check that type renaming via module use is properly handled.

module first
  implicit none
  type t
    integer, pointer :: x(:)
  end type t
  !$omp declare mapper(t :: v) map(v, v%x)
end module

module second
  implicit none
  type t
    integer, pointer :: y(:)
    integer, pointer :: z(:)
  end type t
  !$omp declare mapper(t :: v) map(v, v%y, v%z)
end module

subroutine sub2
 use first, t1 => t
 use second, t2 => t
 implicit none
  type(t1) :: var
  type(t2) :: var2

  allocate(var%x(1:20))
  allocate(var2%y(1:3), var2%z(5))
  var%x = 9
  var2%y = 1
  var2%z = 43

  !$omp target map(tofrom: var) ! { dg-error "Sorry, declared mapper 'default', used for 'var' at .1., is not yet supported" }
  block
    var%x = 4
  end block

  !$omp target map(tofrom: var2) ! { dg-error "Sorry, declared mapper 'default', used for 'var2' at .1., is not yet supported" }
  block
    var2%y = 3
    var2%z = 7
  end block
end
