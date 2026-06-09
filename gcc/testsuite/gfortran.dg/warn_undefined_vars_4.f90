! { dg-do compile }
! { dg-options "-Wundefined-vars" }
program memain
  integer, dimension(10) :: e
  call s1(e) ! { dg-warning "Undefined.*passed to INTENT.IN. argument" }
contains
  subroutine s1(in)
    integer, dimension(:), intent(in) :: in
    print *,in
  end subroutine s1
end program memain

subroutine s3(a, b)
  integer, intent(out) :: a
  integer, intent(inout) :: b
  b = a + 1 ! { dg-warning "Undefined INTENT.OUT. variable" }
end subroutine s3
