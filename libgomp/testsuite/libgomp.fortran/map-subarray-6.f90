! { dg-do run }

use iso_c_binding
use omp_lib
implicit none

type t
  integer, pointer :: p(:)
  integer, pointer :: p2(:)
end type t

type(t) :: var
integer, target :: tgt(5), tgt2(1000)
logical :: host_accessible, self_mapping
integer(c_intptr_t) :: ip, ip2

ip = transfer(c_loc(tgt), ip)
ip2 = transfer(c_loc(tgt2), ip2)

var%p => tgt
var%p2 => tgt2

! 'map(var)' shall never do any pointer attachment - even if the pointer
! variables are mapped to the device.
!
! There are three case:
!
! 1. Host fallback or self mapping: In the target region, the values are modified on the host
! 2. Host memory inaccessible: var's pointers point to the host version (inaccessible);
!    tgt and tgt2 are mapped from/to the device but are inaccessible via var's pointers.
! 3. No selfmapping but variables are accessible on the host:
!    Modifying var%p and var%p2 changes the value on the host - but copying back tgt and
!    tgt2 will set the value prior to the copy in.

! NOTE: Due to PR libgomp/113216, omp_target_is_accessible will report .false. on Nvptx
! even if host access to the device is possible. - This will reduce the test coverage
! but will not cause a fail.

host_accessible = omp_target_is_accessible(c_loc(tgt), c_sizeof(tgt), omp_default_device) /= 0
!host_accessible = .true.  ! set this manually on nvptx with USM support, until fixed

self_mapping = .false.
!$omp target map(to: self_mapping)
  self_mapping = .true.
!$omp end target

print *, (self_mapping ? '' : 'NO '), 'self mapping, ', &
         (host_accessible ? '' : 'NOT '), 'host accessible'

var%p = 1
var%p2 = 2

! The following is slightly tricky as well: As 'tgt' is not used, it is optimized away
! by during gimplification! - Hence, 'tgt' is also not copied back.
! For 'tgt2(4:6)', the expression is too complex for the gimplifier - hence, it
! is copied back ...

!$omp target map(tgt, tgt2(4:6),var) firstprivate(host_accessible, ip, ip2)
  ! No pointer attachment = points to the host
  if (ip /= transfer(c_loc(var%p), ip)) stop 1
  if (ip2 /= transfer(c_loc(var%p2), ip2)) stop 2

  if (lbound(var%p,1) /= 1 .or. ubound(var%p,1) /= 5 .or. .not.associated(var%p)) &
    stop 3
  if (lbound(var%p2,1) /= 1 .or. ubound(var%p2,1) /= 1000 .or. .not.associated(var%p2)) &
    stop 4

  if (omp_is_initial_device() .or. host_accessible) then
    if (any (var%p /= 1)) stop 5
    if (any (var%p2 /= 2)) stop 6
    var%p(1) = 5
    var%p2(5) = 6
    var%p2(22) = 7
  end if
!$omp end target

if (omp_get_default_device() == omp_get_num_devices() &
    .or. omp_get_default_device() == omp_initial_device &
    .or. host_accessible) then
  if (self_mapping) then
    if (var%p(1) /= 5) stop 7
    if (var%p2(5) /= 6) stop 8
    if (var%p2(22) /= 7) stop 9
  else
    if (var%p(1) /= 5) stop 10  ! NO copy back - hence, value is 5
    if (var%p2(5) /= 2) stop 11
    if (var%p2(22) /= 7) stop 12
  endif
  if (any (var%p(2:) /= 1)) stop 13
  if (any (var%p2(:4) /= 2) &
      .or. any (var%p2(6:21) /= 2) &
      .or. any (var%p2(23:) /= 2)) &
    stop 14
else
  if (any(var%p /= 1)) stop 15
  if (any(var%p2 /= 2)) stop 16
end if


! The same - but now using tgt / tgt2 inside the region

var%p = 1
var%p2 = 2

!$omp target map(tgt, tgt2(4:6),var) firstprivate(host_accessible, ip, ip2)
  ! No pointer attachment = points to the host
  if (ip /= transfer(c_loc(var%p), ip)) stop 17
  if (ip2 /= transfer(c_loc(var%p2), ip2)) stop 18

  if (omp_is_initial_device() .or. self_mapping) then
    if (.not. c_associated(c_loc(var%p), c_loc(tgt))) stop 19
    if (.not. c_associated(c_loc(var%p2), c_loc(tgt2))) stop 20
  else
    if (c_associated(c_loc(var%p), c_loc(tgt))) stop 21
    if (c_associated(c_loc(var%p2), c_loc(tgt2))) stop 22
  endif

  if (lbound(var%p,1) /= 1 .or. ubound(var%p,1) /= 5 .or. .not.associated(var%p)) &
    stop 23
  if (lbound(var%p2,1) /= 1 .or. ubound(var%p2,1) /= 1000 .or. .not.associated(var%p2)) &
    stop 24

  if (omp_is_initial_device() .or. host_accessible) then
    if (any (var%p /= 1)) stop 25
    if (any (var%p2 /= 2)) stop 26
    var%p(1) = 5
    var%p2(5) = 6
    var%p2(22) = 7
  end if
!$omp end target

if (omp_get_default_device() == omp_get_num_devices() &
    .or. omp_get_default_device() == omp_initial_device &
    .or. host_accessible) then
  if (self_mapping) then
    if (var%p(1) /= 5) stop 27
    if (var%p2(5) /= 6) stop 28
    if (var%p2(22) /= 7) stop 29
  else
    if (var%p(1) /= 1) stop 30  ! NOW tgt is copied back
    if (var%p2(5) /= 2) stop 31
    if (var%p2(22) /= 7) stop 32
  endif
  if (any (var%p(2:) /= 1)) stop 33
  if (any (var%p2(:4) /= 2) &
      .or. any (var%p2(6:21) /= 2) &
      .or. any (var%p2(23:) /= 2)) &
    stop 34
else
  if (any(var%p /= 1)) stop 35
  if (any(var%p2 /= 2)) stop 36
end if

end
