! { dg-do compile }
! { dg-options "-std=f2018" }
!
! PR fortran/125529
! DO CONCURRENT with inline type-spec iterators and DEFAULT(NONE) inside an
! ASSOCIATE block used to ICE (segfault in check_default_none_expr).  The
! bug was that sym->ns->code was used instead of d->code to locate the
! forall_iterator list; for symbols in an ASSOCIATE or enclosing procedure
! namespace the wrong union member was read, giving a garbage pointer.
! Additionally, shadow iterators store their name with a leading underscore
! internally, so the comparison against user-visible names must strip it.

subroutine test_associate_default_none (a, b)
  implicit none
  integer, intent(in)  :: a(10,10)
  integer, intent(out) :: b(10,10)
  do concurrent (integer :: i = 1:10, j = 1:10) &
      default(none) shared(a, b)
    associate (tmp => a(i,j))
      b(i,j) = tmp + 1
    end associate
  end do
end subroutine

! Verify that a genuinely missing variable still gets an error.
subroutine test_missing_var (a, b, c)
  implicit none
  integer, intent(in)  :: a(10,10), c
  integer, intent(out) :: b(10,10)
  do concurrent (integer :: i = 1:10, j = 1:10) &
      default(none) shared(a, b)
    ! { dg-error "not specified in a locality spec" "" { target *-*-* } .-1 }
    b(i,j) = a(i,j) + c   ! { dg-error "not specified in a locality spec" }
  end do
end subroutine
