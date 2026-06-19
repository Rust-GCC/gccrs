use omp_lib
implicit none (type, external)
integer :: devs(1), dists(1)
integer, allocatable :: a_devs(:), a_dists(:)
integer :: i

! GCC specific: invalid numbers return -1
devs(1) = omp_invalid_device; dists(1) = -99
call omp_get_device_distances (size(devs), devs, dists)
if (devs(1) /= omp_invalid_device .or. dists(1) /= -1) stop 1

devs(1) = -123; dists(1) = -99
call omp_get_device_distances (size(devs), devs, dists)
if (devs(1) /= -123 .or. dists(1) /= -1) stop 2

devs(1) = omp_get_num_devices () + 1; dists(1) = -99
call omp_get_device_distances (size(devs), devs, dists)
if (devs(1) /= omp_get_num_devices () + 1 .or. dists(1) /= -1) stop 3

do i = omp_initial_device, omp_get_num_devices ()
  devs(1) = i; dists(1) = -99
  call omp_get_device_distances (size(devs), devs, dists)
  if (devs(1) /= i) stop 4
  ! Host == 0 per definition
  ! GCC specific: All others (unknown or not) should have > 0
  ! the spec only requires >= 0
  if (i == omp_initial_device .or. i == omp_get_num_devices ()) then
    if (dists(1) /= 0) stop 5
  else
    if (dists(1) <= 0) stop 6
  end if
end do

allocate(a_devs(omp_initial_device:omp_get_num_devices ()))
a_devs = [(i, i = omp_initial_device, omp_get_num_devices ())]
allocate(a_dists, mold=a_devs)
a_dists = -99

call omp_get_device_distances (size(a_devs), a_devs, a_dists)

do i = lbound (a_devs, 1), ubound (a_devs, 1)
  if (a_devs(i) /= i) stop 7
  ! Host == 0 per definition
  ! GCC specific: All others (unknown or not) should have > 0
  if (i == omp_initial_device .or. i == omp_get_num_devices ()) then
    if (a_dists(i) /= 0) stop 8
  else
    if (a_dists(i) <= 0) stop 9
  end if
  print '(a,i3,a,i0)', 'device', i, ': distance ', a_dists(i)
end do

end
