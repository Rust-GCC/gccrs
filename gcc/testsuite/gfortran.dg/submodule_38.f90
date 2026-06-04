! { dg-do run }
!
! Test fix for a bug where the always_explicit attribute was not copied
! by gfc_copy_attr, causing array descriptors not to be passed when
! calling a dummy procedure with an assumed-shape argument from a
! submodule module procedure.
!
module test_m
   implicit none (type, external)
   interface
      module subroutine call_proc (proc)
         interface
            subroutine proc (a)
               real, intent(in) :: a(:)
            end subroutine proc
         end interface
      end subroutine call_proc
   end interface
end module test_m

submodule (test_m) test_sm
   implicit none (type, external)
contains
   module procedure call_proc
      call proc ([1., 2., .3])
   end procedure call_proc
end submodule test_sm

program test
   use test_m
   implicit none (type, external)
   call call_proc (print_proc)
contains
   subroutine print_proc (a)
      real, intent(in) :: a(:)
      if (size (a, 1) /= 3) stop 1
   end subroutine print_proc
end program test
