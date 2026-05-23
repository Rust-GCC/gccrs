! { dg-do run }
!
! Fix for PR125391 due to :
! gfortran ICE: in gimplify_var_or_parm_decl, at gimplify.cc:3416 (26.2) 
!               in gimplify_var_or_parm_decl, at gimplify.cc:3166 (trunk, 16/5/10) 
!
!
! Contributed by Federico Perini  <federico.perini@gmail.com>
!
module m
  implicit none
  type :: inner
    integer :: x = 0
  contains
    final :: inner_final
  end type

  type :: outer2
    type(inner) :: items(1)
  end type

  type :: outer1
    type(inner), allocatable :: items(:)
  end type

  integer :: ctr = 0

contains
  impure elemental subroutine inner_final(this)
    type(inner), intent(inout) :: this
    ctr = ctr + 1
  end subroutine


  ! Variant A: whole-array component assignment.
  subroutine copy_whole1(lhs, rhs)
    class(outer1), intent(inout) :: lhs
    type(outer1),  intent(in)    :: rhs
    lhs%items = rhs%items   ! ICE!
  end subroutine

  ! Variant B: assignment from an array constructor.
  subroutine make_singleton1(lhs, x)
    class(outer1), intent(inout) :: lhs
    type(inner),  intent(in)    :: x
    lhs%items = [x]         ! ICE!
  end subroutine

  ! Variant A: whole-array component assignment.
  subroutine copy_whole2(lhs, rhs)
    class(outer2), intent(inout) :: lhs
    type(outer2),  intent(in)    :: rhs
    lhs%items = rhs%items
  end subroutine

  ! Variant B: assignment from an array constructor.
  subroutine make_singleton2(lhs, x)
    class(outer2), intent(inout) :: lhs
    type(inner),  intent(in)    :: x
    lhs%items = [x]
  end subroutine
end module

program p
  use m
  type(outer1) :: x
  type(outer2) :: y

  ! Verify that the original problem is fixed
  x%items = [inner(2)]
  if (ctr /= 0) stop 1            ! x%items not allocated
  call make_singleton1 (x, inner(42))
  call copy_whole1 (x, outer1([inner(S)]))
  if (ctr /= 2) stop 2            ! one finalization for each call

  ! Verify that the fix has not broken non-allocatable component references
  y%items = [inner(2)]            ! y%items finalized before assignment
  if (ctr /= 3) stop 3
  call make_singleton2 (y, inner(42))
  call copy_whole2 (y, outer2([inner(42)]))
  if (ctr /= 5) stop 4            ! one finalization for each call

end program
