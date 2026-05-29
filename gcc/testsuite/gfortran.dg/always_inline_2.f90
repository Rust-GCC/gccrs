! { dg-do compile }
!
! ALWAYS_INLINE is incompatible with NOINLINE.  Specifying both for the same
! procedure must warn and drop ALWAYS_INLINE, whether they appear in one
! directive or in separate directives.

subroutine foo()
!GCC$ ATTRIBUTES always_inline, noinline :: foo  ! { dg-warning "ALWAYS_INLINE. at .1. is incompatible with .NOINLINE." }
end subroutine foo

subroutine bar()
!GCC$ ATTRIBUTES always_inline :: bar
!GCC$ ATTRIBUTES noinline :: bar  ! { dg-warning "ALWAYS_INLINE. at .1. is incompatible with .NOINLINE." }
end subroutine bar
