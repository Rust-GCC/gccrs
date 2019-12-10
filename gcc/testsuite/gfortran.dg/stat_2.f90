! { dg-do run }
! { dg-skip-if "" { *-*-mingw* } }
! { dg-options "-std=gnu" }
  character(len=*), parameter :: f = "testfile_stat_2"
  integer :: s1(13), r1, s2(13), r2, s3(13), r3, d(13), rd
  
  open (10,file=f)
  write (10,"(A)") "foo"
  close (10,status="keep")

  open (10,file=f)
  r1 = lstat (f, s1)
  r2 = stat (f, s2)
  r3 = fstat (10, s3)
  rd = stat (".", d)

  if (r1 /= 0 .or. r2 /= 0 .or. r3 /= 0 .or. rd /= 0) STOP 1
  if (any (s1 /= s2) .or. any (s1 /= s3)) STOP 2
  if (s1(5) /= getuid()) STOP 3
! If the test is run in a directory with the sgid bit set or on a filesystem
! mounted with the grpid option, new files are created with the directory's
! gid instead of the user's primary gid, so allow for that.
  if (s1(6) /= getgid() .and. s1(6) /= d(6) .and. getgid() /= 0) STOP 4
  if (s1(8) < 3 .or. s1(8) > 5) STOP 5

  close (10,status="delete")
  end
