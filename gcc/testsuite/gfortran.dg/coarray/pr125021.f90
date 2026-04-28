!{ dg-do run }

! Contributed by Neil Carlson  <neil.n.carlson@gmail.com>
! Test for PR fortran/125021

type box
  integer, allocatable :: data(:)
end type
type(box), allocatable :: buffer[:]

integer :: i, n

allocate(buffer[*])
allocate(buffer%data(1), source=this_image())
sync all

i = 1 + modulo(this_image(), num_images())
n = buffer[i]%data(1)
if (n /= i ) error stop
end

