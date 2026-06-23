! { dg-do run }
!
! PR fortran/125761
!
! Passing an array section of an unlimited-polymorphic array with a CHARACTER
! payload addressed the section data with the wrong element size: the section
! base data pointer was offset using the vptr size (the kind size, 1) rather
! than the _len-corrected element size, so every element but the first was
! wrong.

program p
  character(4), target :: arr(5) = ["aaaa", "bbbb", "cccc", "dddd", "eeee"]
  character(2), target :: arr2(4) = ["xx", "yy", "zz", "ww"]
  class(*), pointer :: ptr(:)

  ptr => arr
  call check (ptr(2:4), ["bbbb", "cccc", "dddd"])   ! contiguous
  call check (ptr(1:5:2), ["aaaa", "cccc", "eeee"]) ! strided

  ptr => arr2
  call check (ptr(2:3), ["yy", "zz"])

contains

  subroutine check (x, expect)
    class(*) :: x(:)
    character(*), intent(in) :: expect(:)
    integer :: i
    select type (x)
     type is (character(*))
       if (len (x) /= len (expect)) stop 1
       if (size (x) /= size (expect)) stop 2
       do i = 1, size (x)
          if (x(i) /= expect(i)) stop 3
       end do
     class default
       stop 4
    end select
  end subroutine

end program p
