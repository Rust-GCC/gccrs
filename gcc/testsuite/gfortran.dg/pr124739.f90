! { dg-do run }
program pr
  integer, parameter :: lun = 42
  character(*), parameter :: lfn = 'fort.42'
  integer :: id = -45
  logical :: pending

! Make sure there is no file with the name we will be using
  open (lun, file=lfn, status='old', iostat=iostat)
  if (iostat /= 0) then
    close (lun, status='delete')
  end if

  open (lun, file=lfn)
  write (lun,*) 'hello world!'
  rewind (lun)

  pending  = .true.
  inquire (file=lfn, id=id, pending=pending)   ! Accepted
  if (pending) stop 20
  pending = .true.
  inquire (file="/no/such/file", id=(id), pending=pending) ! Fixed
  if (pending) stop 23
  pending = .true.
  inquire (unit=lun, id=(15 + 35 - 2*8), pending=pending) ! Fixed
  if (pending) stop 23
  close (lun, status='delete')
end program pr
