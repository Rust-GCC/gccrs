! { dg-do compile }
! { dg-options "-Wundefined-vars" }
program memain
  implicit none
  integer, dimension(:), allocatable :: a
  integer, allocatable :: d
  integer, dimension(10) :: e
  integer :: f, g, h, i
  real, dimension(:), allocatable :: r
  allocate (a(10))
  call s1(a) ! { dg-warning "Allocated.*INTENT.IN." }
  allocate (d, source=42)
  print *,d
  call s2(f) ! { dg-warning "Undefined variable" }
  print *,g  ! { dg-warning "Undefined variable.*used" }
  call s3(h,i)
  print *,i
  call allocit(r)
  print *,r
contains
  subroutine s1(in)
    integer, dimension(:), intent(in) :: in
    print *,in
  end subroutine s1
  subroutine s2(in)
    integer, value :: in
    print *,in
  end subroutine s2
  subroutine allocit(fld)
    real, dimension(:), intent(out), allocatable :: fld
    fld = [1,2,3]
  end subroutine allocit
end program memain

