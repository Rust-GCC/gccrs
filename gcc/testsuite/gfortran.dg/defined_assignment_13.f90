! { dg-do run }
!
! PR fortran/120140 - defined assignment and vector subscripts

!---------------------------------------
! Part 1: Derived from original testcase
!---------------------------------------
module mod1
  implicit none

  type typ1
     integer :: i
   contains
     procedure, pass(y) :: assign_typ1
     generic :: assignment(=) => assign_typ1
  end type typ1

contains

  elemental subroutine assign_typ1(x,y)
    class(typ1), intent(inout) :: x
    class(typ1), intent(in)    :: y
    x%i = y%i + 42
  end subroutine assign_typ1

  subroutine test1(x,y)
    class(typ1), intent(inout) :: x(:)
    class(typ1), intent(in)    :: y(:)
    x([2,1]) = y([1,2])
  end subroutine test1

end module mod1

module mod2
  implicit none

  type typ2
     integer i
  end type typ2

  interface assignment(=)
     module procedure sub
  end interface assignment(=)

contains

  elemental subroutine sub(x, y)
    class(typ2), intent(inout) :: x
    class(typ2), intent(in)    :: y
    x%i = y%i + 42
  end subroutine sub

  subroutine test2(x,y)
    class(typ2), intent(inout) :: x(:)
    class(typ2), intent(in)    :: y(:)
    x([2,1]) = y([1,2])
  end subroutine test2

end module mod2

subroutine pr120140
  use mod1
  use mod2
  implicit none

  type(typ1) :: p1(8), q1(8)
  type(typ2) :: p2(8), q2(8)
  integer    :: j

  p1%i = 0
  q1%i = [(j,j=1,8)]
  call test1 (p1, q1)
  if (any(p1%i /= [44, 43, 0, 0, 0, 0, 0, 0])) then
     print *, p1%i
     stop 1
  end if

  p2%i = 0
  q2%i = [(j,j=1,8)]
  call test2 (p2, q2)
  if (any(p2%i /= [44, 43, 0, 0, 0, 0, 0, 0])) then
     print *, p2%i
     stop 2
  end if
end subroutine pr120140

!---------------------------
! Part 2: Supplemental tests
!---------------------------
module pr120140_extras
  implicit none
  public :: extra_tests
  private

  type t1
     integer :: i = 0
  end type t1

  type t2
     integer :: i = 0
  end type t2

  interface assignment(=)
     module procedure sub1
     module procedure sub2
  end interface assignment(=)

contains

  elemental subroutine sub1 (x, y)
    type(t1), intent(inout) :: x
    type(t1), intent(in)    :: y
    x%i = y%i + 23
  end subroutine sub1

  elemental subroutine sub2 (x, y)
    class(t2), intent(inout) :: x
    class(t2), intent(in)    :: y
    x%i = y%i + 42
  end subroutine sub2

  subroutine extra_tests ()
    integer :: j
    type(t1) :: p1(4), q1(4) = [(t1(j),j=1,4)]
    type(t2) :: p2(4), q2(4) = [(t2(j),j=1,4)]
    integer  :: operm(2) = [2,1]
    integer  :: iperm(2) = [1,2]
    integer  :: expect1(4) = [25,24,0,0]
    integer  :: expect2(4) = [44,43,0,0]

    !-----------------------------------
    ! (1) l.h.s. not depending on r.h.s.
    ! check type and class
    !-----------------------------------
    ! l.h.s. array section, r.h.s. array section
    p1%i       = 0
    p1(2:1:-1) = q1(1:2)
    call check (p1%i, expect1, 11)

    p2%i       = 0
    p2(2:1:-1) = q2(1:2)
    call check (p2%i, expect2, 21)

    p1%i       = 0
    call sub1  (p1(2:1:-1), q1(1:2))
    call check (p1%i, expect1, 31)

    p2%i       = 0
    call sub2  (p2(2:1:-1), q2(1:2))
    call check (p2%i, expect2, 41)

    ! l.h.s. vector indices, r.h.s. array section
    p1%i       = 0
    p1([2,1])  = q1(1:2)
    call check (p1%i, expect1, 12)

    p1%i       = 0
    call sub1  (p1([2,1]), q1(1:2))
    call check (p1%i, expect1, 22)

    p2%i       = 0
    p2([2,1])  = q2(1:2)
    call check (p2%i, expect2, 32)

    p2%i       = 0
    call sub2  (p2([2,1]), q2(1:2))
    call check (p2%i, expect2, 42)

    ! l.h.s. permutation vector, r.h.s. array section
    p1%i       = 0
    p1(operm)  = q1(1:2)
    call check (p1%i, expect1, 13)

    p1%i       = 0
    call sub1  (p1(operm), q1(1:2))
    call check (p1%i, expect1, 23)

    p2%i       = 0
    p2(operm)  = q2(1:2)
    call check (p2%i, expect2, 33)

    p2%i       = 0
    call sub2  (p2(operm), q2(1:2))
    call check (p2%i, expect2, 43)

    ! l.h.s. array section, r.h.s vector indices
    p1%i       = 0
    p1(2:1:-1) = q1([1,2])
    call check (p1%i, expect1, 14)

    p1%i       = 0
    call sub1  (p1(2:1:-1), q1([1,2]))
    call check (p1%i, expect1, 24)

    p2%i       = 0
    p2(2:1:-1) = q2([1,2])
    call check (p1%i, expect1, 34)

    p2%i       = 0
    call sub2  (p2(2:1:-1), q2([1,2]))
    call check (p2%i, expect2, 44)

    ! l.h.s. vector indices, r.h.s vector indices
    p1%i       = 0
    p1([2,1])  = q1([1,2])
    call check (p1%i, expect1, 15)

    p1%i       = 0
    call sub1  (p1([2,1]), q1([1,2]))
    call check (p1%i, expect1, 25)

    p2%i       = 0
    p2([2,1])  = q2([1,2])
    call check (p2%i, expect2, 35)

    p2%i       = 0
    call sub2  (p2([2,1]), q2([1,2]))
    call check (p2%i, expect2, 45)

    !---------------------------------
    ! (2) l.h.s. *depending* on r.h.s.
    ! check type and class
    !---------------------------------
    expect1 = [25,24,3,4]
    expect2 = [44,43,3,4]

    ! l.h.s. array section, r.h.s. array section
    p1%i       = q1%i
    p1(2:1:-1) = p1(1:2)
    call check (p1%i, expect1, 51)

    p2%i       = q2%i
    p2(2:1:-1) = p2(1:2)
    call check (p2%i, expect2, 53)

    p1%i       = q1%i
    call sub1  (p1(2:1:-1), (p1(1:2)))  ! Beware: force evaluation of arg2!
    call check (p1%i, expect1, 52)

    p2%i       = q2%i
    call sub2  (p2(2:1:-1), (p2(1:2)))  ! Beware: force evaluation of arg2!
    call check (p2%i, expect2, 54)

    ! l.h.s. array section, r.h.s vector indices
    p1%i       = q1%i
    p1(2:1:-1) = p1([1,2])
    call check (p1%i, expect1, 61)

    p2%i       = q2%i
    p2(2:1:-1) = p2([1,2])
    call check (p2%i, expect2, 63)

    p1%i       = q1%i
    call sub1  (p1(2:1:-1), (p1([1,2])))
    call check (p1%i, expect1, 62)

    p2%i       = q2%i
    call sub2  (p2(2:1:-1), (p2([1,2])))
    call check (p2%i, expect2, 64)

    ! l.h.s. vector indices, r.h.s. array section
    ! (this part currently disabled because the temporary for the l.h.s.
    ! is not yet implemented properly)
!   p1%i       = q1%i
!   p1([2,1])  = p1(1:2)
!   call check (p1%i, expect1, 71)
!
!   p2%i       = q2%i
!   p2([2,1])  = p2(1:2)
!   call check (p2%i, expect2, 73)

!   p1%i       = q1%i
!   call sub1  (p1([2,1]), (p1(1:2)))
!   call check (p1%i, expect1, 72)
!
!   p2%i       = q2%i
!   call sub2  (p2([2,1]), (p2(1:2)))
!   call check (p2%i, expect2, 74)

  end subroutine extra_tests

  subroutine check (result, expect, code)
    integer, intent(in) :: result(:), expect(:), code
    if (any (result /= expect)) then
       print *, code, ":", result, "/=", expect
       stop code
    end if
  end subroutine check

end module pr120140_extras

!-----------

program main
  use pr120140_extras
  call pr120140 ()
  call extra_tests ()
end
