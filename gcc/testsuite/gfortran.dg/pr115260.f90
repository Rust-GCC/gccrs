! { dg-do run }
! PR 115260 - this got corrupted output.
! Original test case by Steve Mullerworth

program usegnufields
  implicit none
  type :: field_type
    real,    allocatable :: data(:)
    integer, allocatable :: idata(:)
  end type field_type
  type :: fieldholder
    type(field_type) :: fieldset(2,4)
  end type fieldholder
  type(fieldholder) :: myfields

  allocate(myfields%fieldset(2,1)%data(3))
  allocate(myfields%fieldset(2,1)%idata(3))
  myfields%fieldset(2,1)%data =1.0
  myfields%fieldset(2,1)%idata=2

  call setfields (myfields%fieldset(2,1:4))
!  print *,'After calling setfields with fieldset(2,1:4)'
!  print *,myfields%fieldset(2,1)%data
!  print *,myfields%fieldset(2,1)%idata
  if (any (myfields%fieldset(2,1)%data  /= 1.0)) stop 1
  if (any (myfields%fieldset(2,1)%idata /= 2  )) stop 2

contains
  subroutine setfields (fieldset)
    type(field_type), intent(inout) :: fieldset(1:4) ! corruption with -O
!    print *,'In setfields:'
!    print *,fieldset(1)%data
!    print *,fieldset(1)%idata
  end subroutine setfields
end
