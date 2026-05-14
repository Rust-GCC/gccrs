! { dg-do compile }
module api
  implicit none

  interface
    function f1(a) result(istat) & ! { dg-error "Type mismatch in argument" }
             bind(c, name="foo")
      use, intrinsic :: iso_c_binding, only :c_int
      implicit none
      integer(kind=c_int) :: a
      integer(kind=c_int) :: istat
    end function
  end interface

  interface
    function f2(a) result(istat) & ! { dg-error "Type mismatch in argument" }
             bind(c, name="foo")
      use, intrinsic :: iso_c_binding, only : c_int, c_float
      implicit none
      real(kind=c_float) :: a  ! integer
      integer(kind=c_int)      :: istat
    end function
 end interface

 interface
    subroutine s3(a) bind(C, name="bar")  ! { dg-error "VALUE attribute" }
      use, intrinsic :: iso_c_binding, only :c_int
      integer, value :: a
    end subroutine s3
 end interface

 interface
    subroutine s4(a) bind(C, name="bar")  ! { dg-error "VALUE attribute" }
      use, intrinsic :: iso_c_binding, only :c_int
      integer :: a
    end subroutine s4
 end interface

 interface
    function f5 (a) result(istat) bind(c, name="qux") ! { dg-error "Return type mismatch" }
      use, intrinsic :: iso_c_binding, only : c_intptr_t, c_int
      implicit none
      integer(kind=c_intptr_t) :: a  ! integer
      integer(kind=c_int)      :: istat
    end function f5
  end interface

  interface
     function f6(a) result(istat) bind(c, name="qux")  ! { dg-error "Return type mismatch" }
      use, intrinsic :: iso_c_binding, only : c_intptr_t, c_float
      implicit none
      integer(kind=c_intptr_t) :: a  ! integer
      real(kind=c_float)      :: istat
    end function f6
  end interface

  interface
     subroutine s7() bind(c, name="quux") ! { dg-error "Too many arguments" }
     end subroutine s7
  end interface

  interface
     subroutine s8(a) bind(c, name="quux")  ! { dg-error "Too many arguments" }
       use, intrinsic :: iso_c_binding, only : c_int
       integer(c_int) :: a
     end subroutine s8
  end interface

  interface
     subroutine s9(a) bind(c, name="quuux")  ! { dg-error "Too many arguments" }
       use, intrinsic :: iso_c_binding, only : c_int
       integer(c_int) :: a
     end subroutine s9
  end interface

  interface
     subroutine s10() bind(c, name="quuux")  ! { dg-error "Too many arguments" }
     end subroutine s10
  end interface

  ! "bla" check with -pedantic only.

  interface
     subroutine s11(a, n) bind(c, name="bla")  ! { dg-error "Rank mismatch" }
       use, intrinsic :: iso_c_binding, only : c_int
       integer(c_int), value :: n
       integer(c_int) :: a
     end subroutine s11
  end interface

  interface
     subroutine s12(a, n) bind(c, name="bla") ! { dg-error "Rank mismatch" }
       use, intrinsic :: iso_c_binding, only : c_int
       integer(c_int), value :: n
       integer(c_int), dimension(*) :: a
     end subroutine s12
  end interface

  interface
     subroutine s14(a) bind(c, name="blubb") ! { dg-error "Type mismatch in argument" }
       use, intrinsic :: iso_c_binding, only: c_ptr
       type(c_ptr), value :: a
     end subroutine s14
  end interface

  interface
     subroutine s15(a) bind(c, name="blubb")  ! { dg-error "Type mismatch in argument" }
       use, intrinsic :: iso_c_binding, only : c_int
       integer(c_int) :: a
     end subroutine s15
  end interface

  interface
     subroutine s16(a) bind(c, name="blabla") ! { dg-error "Array specification mismatch" }
       use, intrinsic :: iso_c_binding, only : c_int
       integer, dimension(:) :: a
     end subroutine s16
  end interface

  interface
     subroutine s17(a) bind(c, name="blabla")  ! { dg-error "Array specification mismatch" }
       use, intrinsic :: iso_c_binding, only : c_int
       integer, dimension(*) :: a
     end subroutine s17
  end interface

end module
