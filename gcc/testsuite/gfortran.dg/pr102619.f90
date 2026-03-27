! { dg-do run }
!
! Test the fix for PR102619.
!
! Contributed by Gerhard Steinmetz  <gscfq@t-online.de>
!
program p
  implicit none
  real :: w(2,3)
  real, allocatable :: y(:)
  y = h(w)                        ! Caused an ICE
  if (size (y) /= product (shape (w))) stop 1
  if (any (int (y) /= [1,2,3,4,5,6])) stop 2
  deallocate (y)
contains
  function h(x) result (g)
    real :: x(..)
    real :: g(product(shape(x)))
    integer :: i
    if (any (shape (x) /= shape (w))) stop 3
    g = [(real(i),i=1,size(g))]
  end
end
