! Ensure that a someone sensible error message is printed

type t
end type t
!$omp declare reduction(+:t)
! { dg-error "28: Syntax error in statement at .1." "" { target *-*-* } .-1 }
! { dg-bogus "Unclassifiable OpenMP directive" "" { target *-*-* } .-2 }


end
