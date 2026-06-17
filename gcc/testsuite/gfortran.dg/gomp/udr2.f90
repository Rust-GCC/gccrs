! { dg-do compile }

subroutine f6
!$omp declare reduction (foo:real:omp_out (omp_in)) ! { dg-error "34: Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }
!$omp declare reduction (bar:real:omp_out = omp_in * omp_out) &
!$omp & initializer (omp_priv (omp_orig)) ! { dg-error "Expected either 'omp_priv = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }
end subroutine f6
subroutine f7
  integer :: a
!$omp declare reduction (foo:integer:a (omp_out, omp_in)) ! { dg-error "37: Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }
!$omp declare reduction (bar:real:omp_out = omp_out.or.omp_in) ! { dg-error "Operands of logical operator" }
!$omp declare reduction (baz:real:omp_out = omp_out + omp_in)
!$omp & initializer (a (omp_priv, omp_orig)) ! { dg-error "Unclassifiable OpenMP directive" }
  real :: r
  r = 0.0
!$omp parallel reduction (bar:r)
!$omp end parallel
end subroutine f7
subroutine f8
  interface
    subroutine f8a (x)
      integer :: x
    end subroutine f8a
  end interface
!$omp declare reduction (baz:integer:omp_out = omp_out + omp_in) &
!$omp & initializer (f8a (omp_orig)) ! { dg-error "One of actual subroutine arguments in INITIALIZER clause" }
!$omp declare reduction (foo:integer:f8a) ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }
!$omp declare reduction (bar:integer:omp_out = omp_out - omp_in) &
!$omp & initializer (f8a) ! { dg-error "Expected either 'omp_priv = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }
end subroutine f8
subroutine f9
  type dt	! { dg-error "which is not consistent with the CALL" }
    integer :: x = 0
    integer :: y = 0
  end type dt
  integer :: i
!$omp declare reduction (foo:integer:dt (omp_out, omp_in)) ! { dg-error "which is not consistent with the CALL" }
!$omp declare reduction (bar:integer:omp_out = omp_out + omp_in) &
!$omp & initializer (dt (omp_priv, omp_orig)) ! { dg-error "which is not consistent with the CALL" }
  i = 0
!$omp parallel reduction (foo : i)
!$omp end parallel
!$omp parallel reduction (bar : i)
!$omp end parallel
end subroutine f9
subroutine f10
  integer :: a, b
!$omp declare reduction(foo:character(len=64) &
!$omp & :omp_out(a:b) = omp_in(a:b)) ! { dg-error "Variable other than OMP_OUT or OMP_IN used in combiner" }
!$omp declare reduction(bar:character(len=16) &
!$omp & :omp_out = trim(omp_out) // omp_in) &
!$omp & initializer (omp_priv(a:b) = ' ') ! { dg-error "Variable other than OMP_PRIV or OMP_ORIG used in INITIALIZER clause" }
end subroutine f10
