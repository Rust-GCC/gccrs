! { dg-do run }

program main
  implicit none

  !$omp requires self_maps

  integer :: i
  INTEGER, POINTER :: fptr(:)
  INTEGER, ALLOCATABLE :: alloc_array(:)
  integer, parameter :: N = 5

  ALLOCATE(fptr(N))
  fptr = 7
  alloc_array = [1,2,3,4,5,6]

  !$omp target firstprivate(fptr, alloc_array)
    DO i=1, N
      fptr(i) = 5*i + fptr(i)
    END DO
    fptr => null() ! ptr must be privatized, pointer target not
    if (any (alloc_array /= [1,2,3,4,5,6])) stop 1
    alloc_array = alloc_array * 21
  !$omp end target

  ! pointer array: values shall be updated
  if (any (fptr /= 7 + 5*[1,2,3,4,5])) stop 2
  ! allocatables: shall not be updated
  if (any (alloc_array /= [1,2,3,4,5,6])) stop 3

  ! Check data-sharing constructs as well:
  !$omp parallel firstprivate(fptr, alloc_array)
    !$omp masked
      fptr = fptr * 10
      fptr => null()
      if (any (alloc_array /= [1,2,3,4,5,6])) stop 4
      alloc_array = alloc_array * 21
    !$omp end masked
  !$omp end parallel

  if (any (fptr /= (7 + 5*[1,2,3,4,5])*10)) stop 5
  if (any (alloc_array /= [1,2,3,4,5,6])) stop 6

  call assumed_shape(alloc_array, N)

  DEALLOCATE(fptr, alloc_array)

contains
  subroutine assumed_shape(x, m)
    integer, value :: m
    integer :: x(:)
    integer :: y(m)
    integer, save :: z(5) 

    y = [11,22,33,44,55]
    z = [111,222,333,444,555]
    !$omp target firstprivate(x,y,z)
      if (any (x /= [1,2,3,4,5,6])) stop 7
      if (any (y /= [11,22,33,44,55])) stop 8
      if (any (z /= [111,222,333,444,555])) stop 9
      x = 31 * x
      y = 47 * y
      z = 53 * z
    !$omp end target

    if (any (x /= [1,2,3,4,5,6])) stop 10
    if (any (y /= [11,22,33,44,55])) stop 11
    if (any (z /= [111,222,333,444,555])) stop 12

    !$omp parallel firstprivate(x,y,z) if(.false.)
      if (any (x /= [1,2,3,4,5,6])) stop 13
      if (any (y /= [11,22,33,44,55])) stop 14
      if (any (z /= [111,222,333,444,555])) stop 15
      x = 31 * x
      y = 47 * y
      z = 53 * z
    !$omp end parallel

    if (any (x /= [1,2,3,4,5,6])) stop 16
    if (any (y /= [11,22,33,44,55])) stop 17
    if (any (z /= [111,222,333,444,555])) stop 18
  end subroutine

end program
