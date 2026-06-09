! { dg-do compile }
! { dg-options "-Wundefined-vars" }
program memain
  implicit none
  integer, allocatable :: c
  integer, dimension(10) :: e
  allocate (c)
  print *,c  ! { dg-warning "Allocated.*undefined" }
contains
  subroutine s1(in)
    integer, dimension(:), intent(in) :: in
    print *,in
  end subroutine s1

end program memain
