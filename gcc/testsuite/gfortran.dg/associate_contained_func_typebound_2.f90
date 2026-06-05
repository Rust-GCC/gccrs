! { dg-do run }
! { dg-options "" }
!
! PR fortran/125530
! ASSOCIATE with a contained-function selector must allow type-bound
! (including generic) procedure calls on the associate name, and resolve
! them to the correct target.  Run-time companion to
! associate_contained_func_typebound.f90.

module m
  implicit none
  type :: t
    integer :: n = 0
  contains
    procedure :: direct_call
    procedure :: target_of_generic1
    procedure :: target_of_generic2
    generic   :: generic_call => target_of_generic1, target_of_generic2
  end type
  integer :: ctr = 0
contains
  subroutine direct_call (self)
    class(t), intent(in) :: self
    ctr = ctr + self%n
  end subroutine
  subroutine target_of_generic1 (self)
    class(t), intent(in) :: self
    ctr = ctr + self%n * 10
  end subroutine
  subroutine target_of_generic2 (self, arg)
    class(t), intent(in) :: self
    integer, intent(in) :: arg
    ctr = ctr + (self%n + arg) * 100
  end subroutine
end module

module m2
  use m
  implicit none
contains
  ! Selector is a contained function.
  subroutine test_direct ()
    associate (x => make_t ())
      call x%direct_call ()
    end associate
  contains
    function make_t () result (r)
      type(t) :: r
      r%n = 1
    end function
  end subroutine

  ! Selector is the module-scope function; generic resolved by argument count.
  subroutine test_generic ()
    associate (x => make_t ())
      call x%generic_call ()
      call x%generic_call (1)
    end associate
  end subroutine

  function make_t () result (r)
    type(t) :: r
    r%n = 2
  end function
end module

program p
  use m
  use m2
  call test_generic    ! 2*10 + (2+1)*100 = 320
  call test_direct     ! + 1            = 321
  if (ctr /= 321) stop 1
end program p
