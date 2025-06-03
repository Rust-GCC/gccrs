! See also 'self.f95'.

! { dg-do compile } 

program test
  implicit none

  logical :: x
  integer :: i

  !$acc parallel if ! { dg-error "Expected '\\(' after 'if'" }
  !$acc parallel if () ! { dg-error "Invalid character" }
  !$acc parallel if (i) ! { dg-error "scalar LOGICAL expression" }
  !$acc end parallel 
  !$acc parallel if (1) ! { dg-error "scalar LOGICAL expression" }
  !$acc end parallel 

  !$acc kernels if ! { dg-error "Expected '\\(' after 'if'" }
  !$acc kernels if () ! { dg-error "Invalid character" }
  !$acc kernels if (i) ! { dg-error "scalar LOGICAL expression" }
  !$acc end kernels 
  !$acc kernels if (1) ! { dg-error "scalar LOGICAL expression" }
  !$acc end kernels

  !$acc serial if ! { dg-error "Expected '\\(' after 'if'" }
  !$acc serial if () ! { dg-error "Invalid character" }
  !$acc serial if (i) ! { dg-error "scalar LOGICAL expression" }
  !$acc end serial 
  !$acc serial if (1) ! { dg-error "scalar LOGICAL expression" }
  !$acc end serial 

  !$acc data if ! { dg-error "Expected '\\(' after 'if'" }
  !$acc data if () ! { dg-error "Invalid character" }
  !$acc data if (i) ! { dg-error "scalar LOGICAL expression" }
  !$acc end data 
  !$acc data if (1) ! { dg-error "scalar LOGICAL expression" }
  !$acc end data 

  ! at most one if clause may appear
  !$acc parallel if (.false.) if (.false.) { dg-error "Duplicated 'if' clause" }
  !$acc kernels if (.false.) if (.false.) { dg-error "Duplicated 'if' clause" }
  !$acc serial if (.false.) if (.false.) { dg-error "Duplicated 'if' clause" }
  !$acc data if (.false.) if (.false.) { dg-error "Duplicated 'if' clause" }

  !$acc parallel if (x)
  !$acc end parallel
  !$acc parallel if (.true.)
  !$acc end parallel
  !$acc parallel if (i.gt.1)
  !$acc end parallel

  !$acc kernels if (x)
  !$acc end kernels
  !$acc kernels if (.true.)
  !$acc end kernels
  !$acc kernels if (i.gt.1)
  !$acc end kernels

  !$acc serial if (x)
  !$acc end serial
  !$acc serial if (.true.)
  !$acc end serial
  !$acc serial if (i.gt.1)
  !$acc end serial

  !$acc data if (x)
  !$acc end data
  !$acc data if (.true.)
  !$acc end data
  !$acc data if (i.gt.1)
  !$acc end data

end program test
