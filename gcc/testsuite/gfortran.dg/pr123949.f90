! { dg-do compile }
!
! PR fortran/123949
! ICE in gfc_match_decl_type_spec with large KIND parameter values.
!
module pr123949_mod
  implicit none
  integer(8), parameter :: big_k = 2147483647_8 + 12345_8

  type cell(k)
    integer(8), kind :: k
    integer(8) :: stamp = -k
  end type

  type(cell(big_k)) :: base
contains
  subroutine make_item(item)
    type(cell(big_k)), allocatable, intent(out) :: item
    allocate (cell(big_k) :: item)
  end subroutine
end module

program pr123949
  use pr123949_mod
  implicit none
  type(cell(big_k)), allocatable :: dyn
  call make_item(dyn)
end program
