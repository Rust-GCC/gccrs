! { dg-do compile }
! { dg-options -Wno-pedantic }
! Special checks which are disabled without -pedantic.

module api
  implicit none

  ! Case insensitivity with different names.

  interface
     subroutine s9(a) bind(c, name="Quuux")
       use, intrinsic :: iso_c_binding, only : c_int
       integer(c_int) :: a
     end subroutine s9
  end interface

  interface
     subroutine s10() bind(c, name="quuux")
     end subroutine s10
  end interface

  interface
     subroutine s11(a, n) bind(c, name="bla")
       use, intrinsic :: iso_c_binding, only : c_int
       integer(c_int), value :: n
       integer(c_int) :: a
     end subroutine s11
  end interface

  interface
     subroutine s12(a, n) bind(c, name="bla")
       use, intrinsic :: iso_c_binding, only : c_int
       integer(c_int), value :: n
       integer(c_int), dimension(*) :: a
     end subroutine s12
  end interface

  interface
     subroutine s13(a, n) bind(c, name="bla")
       use, intrinsic :: iso_c_binding, only : c_int
       integer(c_int), value :: n
       integer(c_int), dimension(n) :: a
     end subroutine s13
  end interface

end module
