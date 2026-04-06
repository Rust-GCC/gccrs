! {dg-do compile }
! { dg-options "-fdump-tree-original" }
!
! Test the fix for PR124598.
!
! Contributed by Antoine Lemoine  <antoine.lemoine@bordeaux-inp.fr>
!
program test_pdt
   implicit none

   type t_foo
      integer :: c
   end type t_foo

   type t_bar(n)
      integer, len :: n = 1
      type(t_foo) :: foo(n)
   end type t_bar

   type t_baz(n)
      integer, len :: n = 1
      type(t_bar(n)) :: bar(n)
   end type t_baz

   type(t_baz(n=10)) :: baz         ! Used to ICE at this line
!   baz%bar(1)%foo = t_foo(1)
   print *, baz%bar(1)%foo
end program test_pdt
! Check that the upper bound for baz%bar%foo is set correctly.
! { dg-final { scan-tree-dump-times "foo.dim.0..ubound = 10" 1 "original" } }
