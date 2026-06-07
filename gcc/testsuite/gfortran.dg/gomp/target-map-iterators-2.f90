! { dg-do compile }
! { dg-options "-fopenmp -fdump-tree-gimple" }

program main
  implicit none

  integer, parameter :: DIM = 40
  type :: array_ptr
    integer, pointer :: ptr(:)
  end type
  
  type (array_ptr) :: x(DIM), y(DIM), z(DIM)

  !$omp target map(iterator(i=1:10), to: x)
    ! Add a reference to x to ensure that the 'to' clause does not get dropped.
    x(1)%ptr(1) = 0
  !$omp end target

  !$omp target map(iterator(i2=1:10, j2=1:20), from: x(i2))
  !$omp end target

  !$omp target map(iterator(i3=1:10, j3=1:20, k3=1:30), to: x(i3+j3), y(j3+k3), z(k3+i3))
  !$omp end target
end program

! { dg-final { scan-tree-dump-times "map\\\(to:x" 1 "gimple" } }
! { dg-final { scan-tree-dump-times "map\\\(iterator\\\(integer\\\(kind=4\\\) i2=1:10:1, loop_label=\[^\\\)\]+\\\):from:" 1 "gimple" } }
! { dg-final { scan-tree-dump-times "map\\\(iterator\\\(integer\\\(kind=4\\\) j3=1:20:1, integer\\\(kind=4\\\) i3=1:10:1, loop_label=\[^\\\)\]+\\\):to:" 1 "gimple" } }
! { dg-final { scan-tree-dump-times "map\\\(iterator\\\(integer\\\(kind=4\\\) k3=1:30:1, integer\\\(kind=4\\\) j3=1:20:1, loop_label=\[^\\\)\]+\\\):to:" 1 "gimple" } }
! { dg-final { scan-tree-dump-times "map\\\(iterator\\\(integer\\\(kind=4\\\) k3=1:30:1, integer\\\(kind=4\\\) i3=1:10:1, loop_label=\[^\\\)\]+\\\):to:" 1 "gimple" } }
