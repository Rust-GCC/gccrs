! { dg-do run }

! PR fortran/121972

! A structure constructor used as an array-constructor element whose type has
! allocatable components leaked those components.  gfc_trans_array_ctor_element
! freed alloc comps only for EXPR_FUNCTION elements; EXPR_STRUCTURE was missed.
! gfc_trans_alloc_subarray_assign also nulled the temp descriptor data pointer
! without first freeing the element alloc comps.  Checked under -fsanitize=address.

module m
  implicit none
  type string_t
    character(len=:), allocatable :: s
  end type
  type file_t
    type(string_t), allocatable :: lines(:)
  end type
contains
  subroutine check (line)
    character(len=*), intent(in) :: line
    type(file_t) :: f
    f = file_t([string_t(line)])
    if (.not. allocated (f%lines)) stop 1
    if (size (f%lines) /= 1) stop 2
    if (.not. allocated (f%lines(1)%s)) stop 3
    if (f%lines(1)%s /= line) stop 4
  end subroutine
end module

program structure_constructor_alloc_comp_leak_1
  use m
  call check ('hello')
  call check ('world')
  call check ('a longer string value')
end program
