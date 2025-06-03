! { dg-do run }
!
! PR fortran/18918
!
! Check whether assumed-shape's cobounds are properly handled
!
    implicit none
    integer :: B(1)[*]
    integer :: C(8:11)[-3:10,43:*]
    integer, allocatable :: D(:)[:,:]

    allocate (D(20)[2:3,5:*])

    call sub (B,5)
    call sub (C,3)
    call sub (D,3)

    call sub2 (B, -3)
    call sub2 (C, 44)
    call sub2 (D, 44)

    call sub3 (B)
    call sub3 (C)
    call sub3 (D)

    call sub4 (B)
    call sub4 (C)
    call sub4 (D)

    call sub5 (D)
  contains

  subroutine sub(A,n)
    integer :: n
    integer :: A(n:)[n:2*n,3*n:*]
    if (lbound(A,dim=1) /= n) STOP 1
    if (any (lcobound(A) /= [n, 3*n])) STOP 2
    if (ucobound(A, dim=1) /= 2*n) STOP 3
  end subroutine sub

  subroutine sub2(A,n)
    integer :: n
    integer :: A(:)[-n:*]
    if (lbound(A,dim=1) /= 1) STOP 4
    if (lcobound(A, dim=1) /= -n) STOP 5
  end subroutine sub2

  subroutine sub3(A)
    integer :: A(:)[0,*]
    if (lbound(A,dim=1) /= 1) STOP 6
    if (lcobound(A, dim=1) /= 1) STOP 7
    if (ucobound(A, dim=1) /= 0) STOP 8
    if (lcobound(A, dim=2) /= 1) STOP 9
  end subroutine sub3

  subroutine sub4(A)
    integer :: A(:)[*]
    if (lbound(A,dim=1) /= 1) STOP 10
    if (lcobound(A, dim=1) /= 1) STOP 11
  end subroutine sub4

  subroutine sub5(A)
    integer, allocatable :: A(:)[:,:]

    if (lbound(A,dim=1) /= 1) STOP 12
    if (lcobound(A, dim=1) /= 2) STOP 13
    if (ucobound(A, dim=1) /= 3) STOP 14
    if (lcobound(A, dim=2) /= 5) STOP 15

    call sub4(A)  ! Check PR88624 is fixed.
  end subroutine sub5
  end
