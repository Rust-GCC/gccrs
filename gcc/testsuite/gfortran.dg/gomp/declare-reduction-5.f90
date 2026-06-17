module m
implicit none
contains
 integer function my_add(x,y)
   integer, value, intent(in) :: x, y
   my_add = x + y
 end
end module

use m
implicit none

type t
  integer :: x
end type t

!$omp declare reduction ! { dg-error "Expected '\\('" }

!$omp declare reduction initializer() ! { dg-error "Expected '\\('" }


!$omp declare reduction ( ! { dg-error "Expected an identfifier or operator as reduction identifier followed by a colon" }

!$omp declare reduction (+      ! { dg-error "Expected an identfifier or operator as reduction identifier followed by a colon" }
!$omp declare reduction (.or.   ! { dg-error "The name 'or' cannot be used as a defined operator" }
!$omp declare reduction (myName ! { dg-error "Expected an identfifier or operator as reduction identifier followed by a colon" }

!$omp declare reduction (+  :     ! { dg-error "Expected type spec" }
!$omp declare reduction (.or. :   ! { dg-error "Expected type spec" }
!$omp declare reduction (myName : ! { dg-error "Expected type spec" }

!$omp declare reduction (+      : t               ! { dg-error "Expected ':' or ','" }
!$omp declare reduction (.or.   : integer         ! { dg-error "Expected ':' or ','" }
!$omp declare reduction (myName : integer(kind=4) ! { dg-error "Expected ':' or ','" }

!$omp declare reduction (foo : integer(1),  integer(kind=2),  ! { dg-error "Expected type spec at" }
!$omp declare reduction (foo : integer(1),  integer(kind=2)   ! { dg-error "Expected ':' or ','" }

!$omp declare reduction (+      : t               : ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }

!$omp declare reduction (.or.   : integer         : ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }
!$omp declare reduction (myName : integer(kind=4) : ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }

!$omp declare reduction (+      : t               : omp_in   ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }
!$omp declare reduction (.or.   : integer         : omp_priv ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }
!$omp declare reduction (myName : integer(kind=4) : omp_orig ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }

!$omp declare reduction (+      : t               : omp_out ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }
!$omp declare reduction (.or.   : integer         : omp_out ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }
!$omp declare reduction (myName : integer(kind=4) : omp_out ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }

!$omp declare reduction (+      : t               : omp_out%x = ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }
!$omp declare reduction (.or.   : integer         : omp_out =   ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }
!$omp declare reduction (myName : integer(kind=4) : omp_out =   ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }

!$omp declare reduction (+      : t               : omp_out%x = 1 ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }
!$omp declare reduction (.or.   : integer         : omp_out =   1 ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }
!$omp declare reduction (myName : integer(kind=4) : omp_out =   1 ! { dg-error "Expected either 'omp_out = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }

! Odd in terms of semantic but valid
!$omp declare reduction (+      : t               : omp_out%x = 1 ) ! { dg-error "Missing INITIALIZER clause for !.OMP DECLARE REDUCTION of derived type without default initializer" }
!$omp declare reduction (.or.   : integer         : omp_out =   1 )
!$omp declare reduction (myName : integer(kind=4) : omp_out =   1 )

!$omp declare reduction (myName5    : t               : omp_out%x = omp_in%x + omp_out%x ) ! { dg-error "Missing INITIALIZER clause for !.OMP DECLARE REDUCTION of derived type without default initializer" }
!$omp declare reduction (.and.   : integer         : omp_out =   0 + omp_out + omp_in )
!$omp declare reduction (myName2 : integer(kind=4) : omp_out =   omp_out + omp_in )

!$omp declare reduction (myName4      : t               : omp_out%x = my_add(omp_in%x , omp_out%x) + 0 ) ! { dg-error "Missing INITIALIZER clause for !.OMP DECLARE REDUCTION of derived type without default initializer" }
!$omp declare reduction (myName6   : integer         : omp_out =   0 + my_add(omp_out,omp_in) )
!$omp declare reduction (myName3 : integer(kind=4) : omp_out =   my_add(omp_out , omp_in ) )

!$omp declare reduction (* : integer : omp_out = omp_in * omp_out) ! { dg-error "Redefinition of predefined 'operator \\*' in !.OMP DECLARE REDUCTION" }
!$omp declare reduction (max : integer : omp_out = omp_in * omp_out) ! { dg-error "Redefinition of predefined 'max' in !.OMP DECLARE REDUCTIO" }

!$omp declare reduction (my : integer : omp_out = omp_in * omp_out) ! { dg-note "Previous !.OMP DECLARE REDUCTION" }
!$omp declare reduction (my : integer : omp_out = omp_in * omp_out) ! { dg-error "Redefinition of 'my' in !.OMP DECLARE REDUCTION" }

!$omp declare reduction (foo : integer(1),  integer(kind=2), real(kind=8) : omp_out = omp_in * omp_out) nowait ! { dg-error "Unexpected junk at .1." }
!$omp declare reduction (bar : integer : omp_out = omp_in * omp_out) initializer ! { dg-error "Unexpected junk at .1." }

!$omp declare reduction (bar2 : integer : omp_out = omp_in * omp_out) initializer( ! { dg-error "Expected either 'omp_priv = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }

!$omp declare reduction (bar3 : integer : omp_out = omp_in * omp_out) initializer( omp_priv = ! { dg-error "Expected either 'omp_priv = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }

!$omp declare reduction (bar3 : integer : omp_out = omp_in * omp_out) initializer( omp_priv = 1! { dg-error "Expected either 'omp_priv = expr' or 'subroutine-name\\(argument-list\\)' followed by '\\)' at .1." }

!$omp declare reduction (bar3 : integer : omp_out = omp_in * omp_out) initializer( omp_priv = 1) nowait ! { dg-error "Unexpected junk at .1." }

end
