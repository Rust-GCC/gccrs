! { dg-do compile }
! PR fortran/98203
! Contributed by G. Steinmetz <gscfq@t-online.de>
!
! ICE in gfc_trans_omp_do with collapse(2) and non-unit stride.

subroutine s
  integer :: i, j
  !$omp parallel do collapse(2)
  do i = 1, 10, 2
    do j = 1, 10
    end do
  end do
end
