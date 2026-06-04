! { dg-do run }
!
! PR fortran/60576
!
! Stack buffer overflow when passing an assumed-rank type(T) dummy argument
! to an assumed-rank class(T) dummy argument.  The caller only allocates
! storage for dtype.rank dimensions in the descriptor; generating a full
! GFC_MAX_DIMENSIONS copy caused a stack-buffer-overflow detected by ASan.

implicit none
type t
  integer :: i
end type

type(T) :: at(2:3,2:4)
integer :: i = 0

call bar(at)
if (i /= 2) STOP 1

contains
  subroutine bar(x)
    type(t) :: x(..)
    if (lbound(x,1) /= 1 .or. lbound(x,2) /= 1) STOP 2
    if (size(x) /= 6) STOP 3
    if (size(x,1) /= 2 .or. size(x,2) /= 3) STOP 4
    if (ubound(x,1) /= 2 .or. ubound(x,2) /= 3) STOP 5
    i = i + 1
    call foo(x)
  end subroutine
  subroutine foo(x)
    class(t) :: x(..)
    if (lbound(x,1) /= 1 .or. lbound(x,2) /= 1) STOP 6
    if (size(x) /= 6) STOP 7
    if (size(x,1) /= 2 .or. size(x,2) /= 3) STOP 8
    if (ubound(x,1) /= 2 .or. ubound(x,2) /= 3) STOP 9
    i = i + 1
  end subroutine
end
