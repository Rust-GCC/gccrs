! { dg-do compile }
! { dg-additional-options "-std=f2018 -Wcharacter-truncation" }
!
! PR fortran/125393 - checking of passed character length

module test
  implicit none
contains

  subroutine a(string)
    character(len=2) string
  end subroutine a

  subroutine b
    character(len=2)               :: s1
    character(len=2), dimension(2) :: s2
    character(len=1)               :: s3(2)
    character(len=4)               :: s4
    call a(s1)
    call a(s1(1:2))
    call a(s2(1)) ! This gave a bogus warning
    call a(s2(1)(1:2))
    call a(s3(1)) ! { dg-error "Character length of actual argument shorter" }
    call a(s4(1:2))
    call a(c1())  ! { dg-error "Character length of actual argument shorter" }
    call a(c2())
    call a(c3())  ! { dg-warning "Character length of actual argument longer" }
  end subroutine b

  function c1 ()
    character(len=1) :: c1
    c1 = "a"
  end function c1

  function c2 ()
    character(len=2) :: c2
    c2 = "ab"
  end function c2

  function c3 ()
    character(len=3) :: c3
    c3 = "abc"
  end function c3

end module test
