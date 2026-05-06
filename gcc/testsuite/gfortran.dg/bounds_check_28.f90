! { dg-do run }
! { dg-options "-fcheck=bounds" }
!
! PR fortran/124661
! Check that the temporary variables used to prevent redundant data
! references aren't used uninitialized in the bounds checking code.
!
! Original testcase from Jakub Benda <albandil at atlas dot cz>.
! Extended and adapted to the testsuite by Christopher Albert <albert at tugraz dot at>.

module m
  implicit none

  type point_t
    real :: coords(1:3)
  end type point_t

  type intermediary_t
    type(point_t), allocatable :: ac(:)
  end type intermediary_t

  type point_cloud_t
    type(point_t), allocatable :: points(:)
  contains
    procedure :: init_point_cloud
  end type point_cloud_t

  type nested_point_cloud_t
    type(intermediary_t), allocatable :: points(:)
  contains
    procedure :: init_nested_point_cloud
  end type nested_point_cloud_t

contains

  subroutine init_point_cloud (c)
    class(point_cloud_t) :: c
    real :: expected(3)

    expected = [1.0, 2.0, 3.0]
    allocate (c%points(1))
    c%points(1)%coords = expected

    if (c%points(1)%coords(3) /= expected(3)) stop 1
    if (any (c%points(1)%coords /= expected)) stop 2
  end subroutine init_point_cloud

  subroutine init_nested_point_cloud (c)
    class(nested_point_cloud_t) :: c
    real :: expected(3)

    expected = [4.0, 5.0, 6.0]
    allocate (c%points(1))
    allocate (c%points(1)%ac(1))
    c%points(1)%ac(1)%coords = expected

    if (c%points(1)%ac(1)%coords(3) /= expected(3)) stop 3
    if (any (c%points(1)%ac(1)%coords /= expected)) stop 4
  end subroutine init_nested_point_cloud
end module m

program p
  use m, only: nested_point_cloud_t, point_cloud_t
  implicit none
  type(point_cloud_t) :: c
  type(nested_point_cloud_t) :: nested

  call c%init_point_cloud
  call nested%init_nested_point_cloud
end program p
