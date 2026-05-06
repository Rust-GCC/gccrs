! { dg-do compile }
! PR fortran/104827
! Contributed by G. Steinmetz <gscfq@t-online.de>
!
! ICE in gfc_conv_array_constructor_expr with declare variant.

program main
contains
  subroutine f(x)
    integer :: x
  end subroutine
  subroutine variant(x)
    integer :: x
    !$omp declare variant (f: match (construct={parallel}))  ! { dg-error "expected '\\)'" }
  end subroutine
end program
