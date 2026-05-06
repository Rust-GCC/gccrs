! { dg-do compile }
program pr
  integer, parameter :: lun = 42
  character(*), parameter :: lfn = 'fort.42'
  integer :: id = -45
  logical :: pending
  inquire (file=lfn, id=id, pending=pending)   ! Accepted
  inquire (file="/no/such/file", id=(id), pending=pending) ! Fixed
end program pr
