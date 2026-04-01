! { dg-do run }
!
! PR fortran/124652 - passing a procedure pointer to c_funloc
!
! Contributed by Damian Rouson

program proc_ptr_demo
  use iso_c_binding
  implicit none

  ! Define an interface for the type of procedure we are pointing to
  abstract interface
     function compute_interface(x) result(res) bind(c)
       use iso_c_binding
       real(c_float), intent(in), value :: x
       real(c_float)                    :: res
     end function compute_interface
  end interface

  ! Procedure pointers
  procedure(compute_interface), pointer :: original_ptr => null()
  procedure(compute_interface), pointer :: restored_ptr => null()
  type(c_funptr) :: c_address
  real(c_float)  :: expect

  expect = square_it (5.0)
  ! Point to our actual function
  original_ptr => square_it
  if (original_ptr (5.0) /= expect) stop 1

  ! Convert pointers "inline"
  c_address = c_funloc (square_it)
  call c_f_procpointer(c_address, restored_ptr)
  if (.not. associated (original_ptr, restored_ptr)) stop 2
  if (restored_ptr (5.0) /= expect) stop 3

  c_address = c_funloc (original_ptr)
  call c_f_procpointer (c_address, restored_ptr)
  if (.not. associated (original_ptr, restored_ptr)) stop 4
  if (restored_ptr (5.0) /= expect) stop 5

  ! Call contained subroutines to perform the C conversion logic
  call round_trip_conversion_proc (square_it, restored_ptr)
  if (.not. associated (original_ptr, restored_ptr)) stop 6
  if (restored_ptr (5.0) /= expect) stop 7

  call round_trip_conversion_proc (original_ptr, restored_ptr)
  if (.not. associated (original_ptr, restored_ptr)) stop 8
  if (restored_ptr (5.0) /= expect) stop 9

  ! The following used to fail
  call round_trip_conversion_ptr (square_it, restored_ptr)
  if (.not. associated (original_ptr, restored_ptr)) stop 10
  if (restored_ptr (5.0) /= expect) stop 11

  call round_trip_conversion_ptr (original_ptr, restored_ptr)
  if (.not. associated (original_ptr, restored_ptr)) stop 12
  if (restored_ptr (5.0) /= expect) stop 13

contains

  subroutine round_trip_conversion_proc (proc_in, fptr_out)
    procedure(compute_interface)                       :: proc_in
    procedure(compute_interface), pointer, intent(out) :: fptr_out
    type(c_funptr) :: c_address
!   print *, proc_in(1.0)
    c_address = c_funloc (proc_in)
    call c_f_procpointer (c_address, fptr_out)
  end subroutine round_trip_conversion_proc

  subroutine round_trip_conversion_ptr (fptr_in, fptr_out)
    procedure(compute_interface), pointer, intent(in)  :: fptr_in
    procedure(compute_interface), pointer, intent(out) :: fptr_out
    type(c_funptr) :: c_address_s
!   print *, fptr_in(2.0)
    c_address_s = c_funloc (fptr_in)
    call c_f_procpointer (c_address_s, fptr_out)
  end subroutine round_trip_conversion_ptr

  function square_it (x) result(res) bind(c)
    real(c_float), intent(in), value :: x
    real(c_float)                    :: res
    res = x * x
  end function square_it

end program
