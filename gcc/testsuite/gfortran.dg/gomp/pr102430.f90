! { dg-do compile }
! { dg-options "-fopenmp" }
! PR fortran/102430

program p
  integer :: a(2)
  !$omp parallel do linear(a)  ! { dg-error "Sorry, array" }
  do i = 1, 8
    a = a + 1
  end do
end program p
