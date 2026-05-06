! { dg-do run }
!
! PR fortran/93814 - ICE in build_entry_thunks with CHARACTER ENTRY
! and bind(c).
!
! Verify that CHARACTER function results with ENTRY and bind(c) compile
! and execute correctly, covering all combinations of bind(c) on the
! function and its entries.

! Both function and entry have bind(c).
function f1() bind(c)
  character :: f1, g1
  f1 = "a"
  return
  entry g1() bind(c)
  g1 = "b"
end

! Only function has bind(c), entry does not.
function f2() bind(c)
  character(1) :: f2, g2
  f2 = "c"
  return
  entry g2()
  g2 = "d"
end function

! Only entry has bind(c), function does not.
function f3()
  character(1) :: f3, g3
  f3 = "e"
  return
  entry g3() bind(c)
  g3 = "f"
end function

! Neither function nor entry have bind(c) (baseline).
function f4()
  character :: f4, g4
  f4 = "g"
  return
  entry g4()
  g4 = "h"
end

! Integer with bind(c) (should still work).
function f5() bind(c)
  integer :: f5, g5
  f5 = 42
  return
  entry g5() bind(c)
  g5 = 84
end

program p
  interface
    function f1() bind(c)
      character :: f1
    end
    function g1() bind(c)
      character :: g1
    end
    function f2() bind(c)
      character(1) :: f2
    end
    function g2()
      character(1) :: g2
    end
    function f3()
      character(1) :: f3
    end
    function g3() bind(c)
      character(1) :: g3
    end
    function f4()
      character :: f4
    end
    function g4()
      character :: g4
    end
    function f5() bind(c)
      integer :: f5
    end
    function g5() bind(c)
      integer :: g5
    end
  end interface

  if (f1() /= "a") stop 1
  if (g1() /= "b") stop 2
  if (f2() /= "c") stop 3
  if (g2() /= "d") stop 4
  if (f3() /= "e") stop 5
  if (g3() /= "f") stop 6
  if (f4() /= "g") stop 7
  if (g4() /= "h") stop 8
  if (f5() /= 42)  stop 9
  if (g5() /= 84)  stop 10
end
