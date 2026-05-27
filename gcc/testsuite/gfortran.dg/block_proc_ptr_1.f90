! { dg-do run }
! PR fortran/105582
! Procedure pointer declared inside a BLOCK construct in a program that has
! contained procedures used to cause an ICE in convert_nonlocal_reference_op
! (tree-nested.cc) because get_proc_pointer_decl erroneously called
! gfc_add_decl_to_parent_function instead of gfc_add_decl_to_function,
! leaving the decl with DECL_CONTEXT set to NULL instead of the enclosing
! program's function_decl.

module iface_m
  implicit none
  abstract interface
    pure function init_i(x) result(y)
      double precision, intent(in) :: x(:)
      double precision, allocatable :: y(:)
    end function
  end interface
end module

program test_block_proc_ptr
  use iface_m, only : init_i
  implicit none
  integer :: n = 0

  block
    procedure(init_i), pointer :: p
    double precision, allocatable :: vals(:)
    p => linear
    vals = p([1d0, 2d0, 3d0])
    if (any(abs(vals - [1d0, 2d0, 3d0]) > 1d-10)) n = n + 1
  end block

  if (n /= 0) stop 1

contains

  pure function linear(x) result(y)
    double precision, intent(in) :: x(:)
    double precision, allocatable :: y(:)
    y = x
  end function

end program
