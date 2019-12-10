! { dg-do run { target { ! { *-*-mingw* *-*-cygwin* } } } }
! { dg-options "-std=gnu" }
! See PR38956.  Test fails on cygwin when user has Administrator rights
  implicit none
  character(len=*), parameter :: n = "foobar_file_chmod_1"
  integer :: i

  open (10,file=n)
  close (10,status="delete")

  open (10,file=n)
  close (10,status="keep")

  if (access(n,"") /= 0 .or. access(n," ") /= 0 .or. access(n,"r") /= 0 .or. &
      access(n,"R") /= 0 .or. access(n,"w") /= 0 .or. access(n,"W") /= 0) &
    STOP 1

  call chmod (n, "a+x", i)
  if (i == 0) then
    if (access(n,"x") /= 0 .or. access(n,"X") /= 0) STOP 2
  end if

  call chmod (n, "a-w", i)
  if (i == 0 .and. getuid() /= 0) then
    if (access(n,"w") == 0 .or. access(n,"W") == 0) STOP 3
  end if

  open (10,file=n)
  close (10,status="delete")

  if (access(n,"") == 0 .or. access(n," ") == 0 .or. access(n,"r") == 0 .or. &
      access(n,"R") == 0 .or. access(n,"w") == 0 .or. access(n,"W") == 0) &
    STOP 4

  end
