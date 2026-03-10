! { dg-do compile }
! { dg-additional-options "-fdump-tree-original" }
!
! PR fortran/120723 - scalar OpenACC attach/detach must not lower through a
! standalone pointer-mapping node.

implicit none (type, external)
integer, pointer :: a, b(:)
integer, allocatable :: c, d(:)

! Scalar pointer and allocatable items used to ICE here.
!$acc enter data attach(a, c)
!$acc enter data attach(b, d)
!$acc exit data detach(a, c)
!$acc exit data detach(b, d)

! { dg-final { scan-tree-dump-times "(?n)#pragma acc enter data map\\(attach:a \\\[bias: 0\\\]\\) map\\(attach:c \\\[bias: 0\\\]\\);$" 1 "original" } }
! { dg-final { scan-tree-dump-times "(?n)#pragma acc exit data map\\(detach:a \\\[bias: 0\\\]\\) map\\(detach:c \\\[bias: 0\\\]\\);$" 1 "original" } }

end
