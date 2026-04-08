! { dg-do compile }
!
! PR fortran/103367
! Invalid initialization expressions must not ICE if they survive
! semantic checking and reach array initializer lowering.

program p
  type t
    integer :: a(1,2) = 3
  end type
  type(t), parameter :: x(1) = t(4)
  integer :: y(1,2) = (x(b)%a) ! { dg-error "does not reduce to a constant expression" }
  print *, y
end
! { dg-prune-output "Legacy Extension: REAL array index" }
