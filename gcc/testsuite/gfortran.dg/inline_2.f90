! { dg-do compile }
!
! INLINE is incompatible with NOINLINE.  Specifying both for the same
! procedure must warn and drop INLINE, whether they appear in one directive
! or in separate directives.

subroutine foo()
!GCC$ ATTRIBUTES inline, noinline :: foo  ! { dg-warning "INLINE. at .1. is incompatible with .NOINLINE." }
end subroutine foo

subroutine bar()
!GCC$ ATTRIBUTES inline :: bar
!GCC$ ATTRIBUTES noinline :: bar  ! { dg-warning "INLINE. at .1. is incompatible with .NOINLINE." }
end subroutine bar
