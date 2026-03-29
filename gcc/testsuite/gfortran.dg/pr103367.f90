! { dg-do compile }
!
! PR fortran/103367
! An undefined variable in a parameter array initializer reached
! gfc_conv_array_initializer and hit gcc_unreachable().

program p
  type t
    integer :: a(1,2) = 3
  end type
  type(t), parameter :: x(1) = t(4)
  integer :: y(1,2) = (x(b)%a)  ! { dg-warning "Legacy Extension" }
  print *, y
end
