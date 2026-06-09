! { dg-do compile }
! { dg-options "-Wunused-but-set-variable" }
module x
  implicit none
contains
  subroutine tst1
    character(len=100) :: iomsg
    character(len=30) :: outstring
    integer :: ios, u, n
    real :: x
    integer, dimension(:), allocatable :: a1, a2
    inquire (6, iostat=ios) ! { dg-warning "never used" }
    inquire (6, iomsg=iomsg) ! { dg-warning "never used" }
    write (outstring,'(A)') "Hello, world" ! { dg-warning "never used" }
    x = 42. ! { dg-warning "never used" }
    open (newunit=u,file="asdf.txt") ! { dg-warning "never used" }
    n = 10
    allocate(a1(n)) ! { dg-warning "allocated at" }
    allocate(a2(n))
    a2 = 42 ! { dg-warning "never used" }
  end subroutine tst1
  ! Test a few combinations that should not emit warnings
  subroutine tst2
    integer i, n, m,k,l
    integer, target:: it
    integer, pointer :: p
    real, allocatable :: r
    real :: x
    integer, dimension(:), allocatable :: ia

    n = 12
    l = 2
    do i=1,n,l
    end do
    m = 12
    k = 3
    forall (i=1:m:k)
    end forall
    p => it
    p = 42
    print *,p
    allocate(r)
    r = 42.
    print *,r
    x = 42.
    call in(x+4.)
    ia = [1, 2, 3]
    print *,ia
  end subroutine tst2
  subroutine in(x)
    real, intent(in) :: x
    print *,x
  end subroutine in
end module x
