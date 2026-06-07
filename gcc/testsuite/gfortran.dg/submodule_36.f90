! { dg-do compile }
!
! Test the fix for pr121379, in which the missing MODULE prefix for 'realg' was
! not detected.
!
! Contributed by Paul Thomas  <pault@gcc.gnu.org>
!
module m
  interface g
  real module function realg1 (arg1, arg2)
    real, intent(in) :: arg1, arg2
  end
  end interface

  interface h
  real module function realg2 (arg1, arg2)
    real, intent(in) :: arg1, arg2
  end
  end interface

contains
end module m

submodule (m) subm
contains
  real module function realg1 (arg1, arg2)
    real, intent(in) :: arg1, arg2
    realg1 = arg1 + arg2
  end

  real function realg2 (arg1, arg2)  ! { dg-error "requires the MODULE prefix" }
    real, intent(in) :: arg1, arg2
    realg2 = arg1 + arg2
  end
end

  use m
  print *, g(1.0, 1.0), h(2.0, 2.0)
end
