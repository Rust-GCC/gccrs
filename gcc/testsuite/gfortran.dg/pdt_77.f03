! { dg-do run }
! { dg-options "-fdump-tree-original" }
!
! Test the fix for PR110012, which failed to compile with an ICE.
! Later, it was found that mfe_disc_test was leaking memory because
! the 'navier_stokes' component p was given the PDT template dynamic
! type, rather than that of the correct instance. This is detected
! by the added, final subroutine being called twice, rather than
! once (PR121972).
!
! Contributed by Neil Carlson  <neil.n.carlson@gmail.com>
!
module pde_class
  type, abstract :: pde(npde)
    integer,len :: npde
  end type
end module

module navier_stokes_type
  use pde_class
  type, extends(pde) :: navier_stokes
    integer, allocatable :: data_(:)
  contains
    final :: finalIze_navier_stokes
  end type
  integer :: ctr = 0
contains
  subroutine alloc_navier_stokes(p , n)
    class(pde(:)), allocatable :: p
    integer :: n
    allocate(navier_stokes(npde=n) :: p)
    select type (p)
      type is (navier_stokes(*))
        p%data_ = [(i, i = 1, p%npde)]
    end select
  end subroutine
  impure elemental subroutine finalIze_navier_stokes (self)
    type(navier_stokes), intent(inout) :: self
    ctr = ctr + 1
  end
end module

module mfe_disc_type
  use pde_class
  type :: foo
    class(pde(:)), allocatable :: p ! This caused the ICE in resolution.
  end type
end module

program test
  use navier_stokes_type
  call navier_stokes_test
  call mfe_disc_test
  if (ctr /= 2) stop 3
contains
  subroutine navier_stokes_test
    class (pde(:)), allocatable :: x
    call alloc_navier_stokes (x, 4)
    select type (x)
      type is (navier_stokes(*))
        if (any (x%data_ /= [1,2,3,4])) stop 1
    end select
  end subroutine

  subroutine mfe_disc_test
    use mfe_disc_type
    type (foo), allocatable :: x
    allocate (x)
    call alloc_navier_stokes (x%p, 3)
    select type (z => x%p)
      type is (navier_stokes(*))
        if (any (z%data_ /= [1,2,3])) stop 2
    end select
    if (allocated (x) .and. allocated (x%p)) deallocate (x%p)
  end subroutine
end program
! { dg-final { scan-tree-dump-times "VIEW_CONVERT_EXPR" 0 "original" } }
