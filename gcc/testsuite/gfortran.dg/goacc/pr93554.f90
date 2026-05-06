! { dg-do compile }
! { dg-additional-options "-fopenacc" }
!
! PR fortran/93554
! ICE in expand_oacc_for when a derived type with an allocatable
! component is used with the private clause on an acc loop.

program p
   type t
      integer :: a
      integer, allocatable :: b(:)
   end type
   type(t) :: x
   integer :: i
   !$acc kernels
   !$acc loop private(x)
   do i = 0, 31
      x%a = 1
   end do
   !$acc end kernels
end
