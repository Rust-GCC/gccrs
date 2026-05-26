! { dg-do compile }
!
! PR fortran/125481
!
! Verify that passing a procedure pointer actual argument to a plain
! data-object dummy argument is rejected with a clear diagnostic
! (F23:15.5.2.5, para 2).

module m_err
  implicit none

  abstract interface
    function func_t (x) result (y)
      real(8), intent(in) :: x
      real(8) :: y
    end function
  end interface

  type :: t
    procedure(func_t), pointer, nopass :: fp => null()
  end type

contains

  subroutine takes_data (x)
    real(8), intent(in) :: x
    print *, x
  end subroutine

end module m_err

program test
  use m_err
  implicit none
  procedure(func_t), pointer :: f => null()
  type(t) :: obj

  ! Procedure pointer variable passed to a data-object dummy.
  call takes_data (f)       ! { dg-error "cannot be passed to data-object dummy argument" }

  ! Procedure pointer component passed to a data-object dummy.
  call takes_data (obj%fp)  ! { dg-error "cannot be passed to data-object dummy argument" }

end program test
