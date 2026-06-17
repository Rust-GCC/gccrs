! { dg-do compile }

subroutine f1
!$omp declare reduction (.le.:integer:omp_out = omp_out + omp_in) ! { dg-error "Invalid operator for" }
end subroutine f1
subroutine f2
!$omp declare reduction (bar:real(kind=4):omp_out = omp_out + omp_in)
  real(kind=4) :: r
  integer :: i
  r = 0.0
!$omp parallel do reduction (bar:r)
  do i = 1, 10
    r = r + i
  end do
!$omp parallel do reduction (foo:r) ! { dg-error "foo not found" }
  do i = 1, 10
    r = r + i
  end do
!$omp parallel do reduction (.gt.:r) ! { dg-error "cannot be used as a defined operator" }
  do i = 1, 10
    r = r + i
  end do
end subroutine f2
subroutine f3
!$omp declare reduction (foo:blah:omp_out=omp_out + omp_in) ! { dg-error "30: Expected type spec at .1." }
end subroutine f3
subroutine f4
!$omp declare reduction (foo:integer:a => null()) ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }
!$omp declare reduction (foo:integer:omp_out = omp_in + omp_out) &
!$omp & initializer(a => null()) ! { dg-error "Expected either 'omp_priv = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }
end subroutine f4
subroutine f5
  integer :: a, b
!$omp declare reduction (foo:integer:a = b + 1) ! { dg-error "Variable other than OMP_OUT or OMP_IN used in combiner" }
!$omp declare reduction (bar:integer:omp_out = omp_out * omp_in) &
!$omp & initializer(b = a + 1) ! { dg-error "Variable other than OMP_PRIV or OMP_ORIG used in INITIALIZER clause" }
end subroutine f5
subroutine f6
!$omp declare reduction (foo:integer:omp_out=omp_out+omp_in) &
!$omp & initializer(omp_orig=omp_priv)
end subroutine f6
