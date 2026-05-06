! { dg-do compile }

subroutine expa (ip, iq, sabb)
  implicit none
  integer, intent(in) :: ip, iq
  real :: sabb(25, 8), arg(25), argj, dist
  integer :: i, j
  integer :: nuca, nucb, nucab, nuco

  arg = [(i * 7.5, i = 1, 25)]
  nuca = nuco (ip)
  nucb = nuco (iq)
  nucab = nuca * nucb
  dist = 0.7854
  do j = 1, nucab
    argj = min (arg(j) * dist, 80.0)
    sabb(j, 1) = exp (-argj)
  end do
  call acomp (sabb, nuca)
  return
entry rfinit
  stop 1
end subroutine expa

subroutine acomp (saab, nuca)
  implicit none
  real :: aaint, saab
  integer :: i, ip, iq, j, nuc1, nuca
  dimension saab(nuca, nuca), aaint(nuc1, nuc1)

  do i = 2, nuca
    do j = 1, i - 1
      saab(j, i) = saab(j, i) * 2.0d0
      saab(i, j) = 0.0d0
    end do
  end do
  return
entry aaexp (aaint, nuc1)
  do ip = 2, nuc1
    do iq = 1, ip - 1
      aaint(iq, ip) = 0.5d0 * (aaint(iq, ip) + aaint(ip, iq))
    end do
  end do
end subroutine acomp
