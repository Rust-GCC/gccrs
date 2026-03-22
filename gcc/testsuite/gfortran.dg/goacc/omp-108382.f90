! { dg-do compile }
! { dg-additional-options "-fopenmp" }

module m_108382_a
  implicit none
  integer :: x, y

!$acc declare copyin(x, &
!$acc   y)
!$omp declare target(x, &
!$omp   y)
end module m_108382_a

module m_108382_b
  implicit none
  integer :: x, y

!$omp declare target(x, &
!$omp   y)
!$acc declare copyin(x, &
!$acc   y)
end module m_108382_b
