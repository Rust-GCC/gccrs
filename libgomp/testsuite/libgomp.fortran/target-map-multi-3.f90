! Test multiple map clauses for the same variable with the 'present'
! modifier.
! { dg-do run }

program main
  implicit none
  integer, parameter :: N = 100
  integer :: a(N), i

  a = [(i, i=1,N)]

  ! { dg-output "libgomp: present clause: not present on the device \\(addr: 0x\[0-9a-f\]+, size: \[0-9\]+ \\(0x\[0-9a-f\]+\\), dev: \[0-9\]+\\\)" { target offload_device_nonshared_as } }
  ! { dg-shouldfail "present error triggered" { offload_device_nonshared_as } }
  !$omp target map(to: a) map(present, alloc: a) map(from: a)
    a = a * 2
  !$omp end target

end program
