! { dg-do compile }
! { dg-options "" }
!
! PR fortran/125530
! ASSOCIATE with a contained-function selector giving spurious errors when
! type-bound (including generic) procedures are called on the associate name.

module tbp_m
  implicit none
  type :: t
    integer :: n = 0
  contains
    procedure :: direct_call
    procedure :: target_of_generic
    generic   :: generic_call => target_of_generic
  end type
contains
  subroutine direct_call (self)
    class(t), intent(in) :: self
    print *, self%n
  end subroutine
  subroutine target_of_generic (self)
    class(t), intent(in) :: self
    print *, self%n
  end subroutine
end module

! Direct type-bound call.
subroutine test_direct ()
  use tbp_m
  implicit none
  associate (x => make_t())
    call x%direct_call()
  end associate
contains
  function make_t() result(r)
    type(t) :: r
    r%n = 1
  end function
end subroutine

! Generic type-bound call.
subroutine test_generic ()
  use tbp_m
  implicit none
  associate (x => make_t())
    call x%generic_call()
  end associate
contains
  function make_t() result(r)
    type(t) :: r
    r%n = 2
  end function
end subroutine
