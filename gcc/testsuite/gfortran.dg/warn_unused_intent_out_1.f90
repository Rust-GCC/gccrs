! { dg-do compile }
! { dg-additional-options "-Wunused-intent-out" }
module y
  implicit none
contains
    subroutine tst
    real :: a, b
    call intent_out(a) ! { dg-warning "passed to INTENT\\(OUT\\)" }
    call intent_out(b)
    print *,b
  end subroutine tst
  subroutine intent_out(x)
    real, intent(out) :: x
    x = 0.4
  end subroutine intent_out
end module y
