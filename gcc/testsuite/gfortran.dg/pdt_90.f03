! { dg-do compile }
! { dg-options "-fdump-tree-original" }
!
! Check the fix for PR115315.f90 in which line 14 caused the error,
! "Argument of IACHAR at (1) must be of length one".
!
! Contributed by David Binderman  <dcb314@hotmail.com>
!
  call p2
contains
  subroutine p2
    type t1(n1,n2)
      integer,kind :: n1,n2
      integer :: c2(iachar('ABCDEFGHIJ'(n1:n2)))
    end type

    type(t1(4,4)) :: x
    if (char (size (x%c2, 1)) .ne. "D") then
      print *, "Wrong!"
    else
      print *, "Right"
    endif
  end
end
! { dg-final { scan-tree-dump-times "Wrong" 0 "original" } }
! { dg-final { scan-tree-dump-times "Right" 1 "original" } }
