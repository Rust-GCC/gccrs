! { dg-do run }
!
! Test the fix for PR124780, which failes as in comments below.
!
! Contributed by Harald Anlauf  <anlauf@gcc.gnu.org>
!
program p
   integer :: i
   type :: t
     integer, allocatable :: i(:)
   end type
   type (t), allocatable :: src(:), ans(:)
   src = [t([1,2]), t([3,4])] ! Leaks memory 16 bytes in 2 blocks;
                              ! familiar from PDT memory leaks :-(
   ans = f(src)
   do i = 1,2
     if (any (src(i)%i /= ans(i)%i)) stop 1
     deallocate (ans(i)%i, src(i)%i)
   enddo
   deallocate (ans, src) 
contains
   function f(x) result(z)
     class(t), intent(inout) :: x(:)
     type(t)  :: z (size(x))
     class(t), allocatable :: a(:)
     class(t), allocatable :: b(:)
     allocate (a(size(x)))
     select type (x)
       type is (t)
         a = x                                ! Mangled src and caused
                                              ! double free at line 12
     end select
     b = x
     z = (b)                                  ! ICE, without patch
   end
end
