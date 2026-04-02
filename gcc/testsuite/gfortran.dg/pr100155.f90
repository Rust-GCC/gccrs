! { dg-do run }
! Test the fix for PR100155 in which the parentheses caused an ICE
! in evaluation the specification expression for 'z'. Note that the
! recursive attribute is not a factor in the ICE (see PR105168).
! Contributed by Gerhard Steinmetz  <gscfq@t-online.de>
!
module m1
   type t
     integer, allocatable :: i
   end type
   integer :: ctr = 0, vals = 0
   integer, parameter :: no_calls = 6
contains
   recursive function f(x) result(z)
      class(t) :: x(:)
      type(t) :: z(size(x)+1)
      class(t), allocatable :: a(:)
      type(t), allocatable :: b(:)
      ctr = ctr + 1
      allocate (t :: a(1))
      a(1)%i = ctr
      if (ctr <= no_calls - 1) then
          b = f((a)) ! <== parentheses
      else
          allocate (b(a(1)%i))
          b(1)%i = ctr
      end if
      vals = vals + b(1)%i
      z(1) = t(b(1)%i)
   end 
end module m1

  use m1
  type (t) :: dummy(1)
  type(t), allocatable :: res(:)
  dummy = t(1)
  res = f (dummy);
  if (ctr /= no_calls) stop 1
  if (vals /= (2 * sum ([(i, i = 1, no_calls)]) - no_calls)) stop 2
  if (size (res) /= 2) stop 3
  deallocate (res)
  deallocate (dummy(1)%i)
end
