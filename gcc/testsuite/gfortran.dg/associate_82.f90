! { dg-do run }
!
! Test the fix for a double free when an ASSOCIATE selector is a function
! returning an allocatable deferred-length character.  The cleanup code
! for the associate name freed the result a second time, after the
! procedure call's own cleanup had already freed it.

module m
  implicit none
contains
  function get_string () result(val)
    character(len=:), allocatable :: val
    val = "needle"
  end function
end module

program main
  use m
  implicit none

  associate (search_string => get_string ())
    if (len (search_string) == 0) STOP 1
    if (search_string // "!" /= "needle!") STOP 2
  end associate
end program
