! { dg-do run }
! PR fortran/125914 - optimize 1.0*n
! { dg-additional-options "-fdump-tree-original" }

program memain
  implicit none
  integer, parameter :: n = 16
  integer (kind=1), dimension(n) :: i1 
  integer (kind=2), dimension(n) :: i2
  integer (kind=4), dimension(n) :: i4 = &
       [6, 5, 4, 4, 10, 8, 12, 8, 9, 4, 8, 11, 12, 10, 3, 12]
  integer (kind=8), dimension(n) :: i8
  real (kind=8), dimension(n) :: r8 = &
       [0.996104819512799_8, 0.245675968329691_8, &
       0.0303468106690589_8, 0.0240992716571487_8,  &
       0.0354064316729468_8, 0.275774313901932_8, &
       0.544792948166924_8, 0.445879110979491_8, &
       0.241491365544482_8, 0.0954139664143259_8, &
       0.0109187857324577_8, 0.0946393553679743_8, &
       0.140308573675276_8, 0.353858089853078_8, &
       0.879882896668272_8, 0.818438744841301_8] 

  real (kind=4), dimension(n) :: r4

  integer(kind=8) :: i
  real (kind=8) :: s8
  real (kind=4) :: s4

  i1 = i4
  i2 = i4
  i8 = i4

  s8 = 0
  do i=1,n
     s8 = s8 + r8(i)*(-1.0_8)**i1(i)
  end do
  if (abs (s8 - 2.3096522811663198_8) > 1e-13_8) stop 1

  s8 = 0
  do i=1,n
     s8 = s8 + r8(i)*(-1.0_8)**i2(i)
  end do
  if (abs (s8 - 2.3096522811663198_8) > 1e-13_8) stop 2

  s8 = 0
  do i=1,n
     s8 = s8 + r8(i)*(-1.0_8)**i4(i)
  end do
  if (abs (s8 - 2.3096522811663198_8) > 1e-13_8) stop 3

  s8 = 0
  do i=1,n
     s8 = s8 + r8(i)*(-1.0_8)**i8(i)
  end do
  if (abs (s8 - 2.3096522811663198_8) > 1e-13_8) stop 4

  r4 = r8

  s4 = 0
  do i=1,n
     s4 = s4 + r4(i)*(-1.0)**i1(i)
  end do
  if (abs (s4 -  2.30965209) > 1e-5) stop 11

  s4 = 0
  do i=1,n
     s4 = s4 + r4(i)*(-1.0)**i2(i)
  end do
  if (abs (s4 -  2.30965209) > 1e-5) stop 12

  s4 = 0
  do i=1,n
     s4 = s4 + r4(i)*(-1.0)**i4(i)
  end do
  if (abs (s4 -  2.30965209) > 1e-5) stop 13

  s4 = 0
  do i=1,n
     s4 = s4 + r4(i)*(-1.0)**i8(i)
  end do
  if (abs (s4 -  2.30965209) > 1e-5) stop 14

end program memain
! { dg-final { scan-tree-dump-not "_gfortran_pow" "original" } }
! { dg-final { scan-tree-dump-not "__builtin_powi" "original" } }
