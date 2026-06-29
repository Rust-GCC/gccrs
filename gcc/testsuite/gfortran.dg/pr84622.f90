! { dg-do compile }
! { dg-options "-fcoarray=single" }
!
! Previously s/CLASS/TYPE at line 12 caught the error.
!
! Contributed by Damian ROuson  <damian@archaeologic.codes>
!
  type foo
    logical, allocatable :: bar[:]
  end type
contains
  subroutine foobar(this) ! { dg-error "or have coarray components" }
    class(foo), intent(out) :: this
  end subroutine
end
