! { dg-do compile }
! PR fortran/95375 - ICE in add_use_op
! Contributed by G.Steinmetz

function f() result(n) bind(c)      ! { dg-error "not C interoperable" }
  ! { dg-error "Return type mismatch" "" { target "*-*-*" } .-1 }
  class(*), allocatable :: n
end
program p
  interface
     function f() result(n) bind(c) ! { dg-error "Return type mismatch" }
       integer :: n
     end
  end interface
  if ( f() /= 0 ) stop
end

