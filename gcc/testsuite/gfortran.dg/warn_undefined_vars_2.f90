! { dg-do compile }
! { dg-options "-Wundefined-vars" }
program memain
  implicit none
  integer, allocatable :: b, c, d
  integer, dimension(10) :: e
  integer :: f, g, h, i
  real, dimension(:), allocatable :: r
  allocate (b)
  call s2(b) ! { dg-warning "Allocated variable.*VALUE" }
contains
  subroutine s2(in)
    integer, value :: in
    print *,in
  end subroutine s2
end program memain

