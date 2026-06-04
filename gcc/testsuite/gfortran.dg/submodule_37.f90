! { dg-do compile }
! { dg-options "-fdump-tree-original" }
!
! Test the fix for pr93424 in which the declaration at line 41 yielded:
! first "Error: Unclassifiable statement at (1)"
! followed by an ICE at interface.cc:851
!
! Contributed by Andrew Benson  <abensonca@gmail.com>
!
module t
  type :: a
   contains
     procedure :: p => ap
  end type a

  type, extends(a) :: b
   contains
     procedure :: p => bp
  end type b

  type :: c
  end type c
  
  interface
     module function bp(s)
       class(b), intent(inout) :: s
       class(c), pointer :: bp
     end function
  end interface
contains
  function ap(s)
    class(a), intent(inout) :: s
    class(c), pointer :: ap
    ap => NULL()
  end function ap
end module t

submodule (t) ts
contains
  function bp(s)
    class(b), intent(inout) :: s
    class(c), pointer :: bp
    select type (s)
      type is (b)
      bp => NULL()
    end select
  end function bp  
end submodule ts

  use t
  class(c), pointer :: x
  type(c), target :: y
  type(b) :: z
  x => y
  if (.not.associated(x,y)) stop 1
  x => z%p()
  if (associated(x,y)) stop 2
end
! { dg-final { scan-tree-dump-times "D..... = bp \\(&class..\\)" 1 "original" } }
