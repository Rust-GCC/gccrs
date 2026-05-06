! { dg-do compile }
! PR fortran/123943
!
! Nested DO CONCURRENT in block constructs must not ICE in gfc_resolve_forall.
! Reduced testcase by Harald Anlauf <anlauf@gcc.gnu.org>

subroutine nested_in_if
  implicit none
  integer :: k, l

  do concurrent (k = 1:5)
    if (k == 3) then
      do concurrent (l = 1:4)
      end do
    end if
  end do
end subroutine nested_in_if


subroutine nested_in_if_else
  implicit none
  integer :: k, l

  do concurrent (k = 1:5)
    if (k == 3) then
      do concurrent (l = 1:4)
      end do
    else
      do concurrent (l = 1:2)
      end do
    end if
  end do
end subroutine nested_in_if_else


subroutine nested_in_select_case
  implicit none
  integer :: k, l

  do concurrent (k = 1:5)
    select case (k)
    case (3)
      do concurrent (l = 1:4)
      end do
    case default
    end select
  end do
end subroutine nested_in_select_case
