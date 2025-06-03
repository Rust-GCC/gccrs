! { dg-do run }
! { dg-options "-fbounds-check" }
! { dg-shouldfail "Incorrect extent in return value of MAXLOC intrinsic: is 3, should be 2|Array bound mismatch for dimension 1 of array 'res' .3/2." }
module tst
contains
  subroutine foo(res)
    integer(kind=4), allocatable :: f(:,:)
    integer, dimension(:) :: res
    allocate (f(2,5))
    f = 3
    res = maxloc(f,mask=.true.)
  end subroutine foo

end module tst
program main
  use tst
  implicit none
  integer :: res(3)
  call foo(res)
end program main
! { dg-output "Fortran runtime error: Incorrect extent in return value of MAXLOC intrinsic: is 3, should be 2|Array bound mismatch for dimension 1 of array 'res' .3/2." }
