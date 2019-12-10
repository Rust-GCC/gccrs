! { dg-do compile }
module Foo_mod

   implicit none

   interface
      module subroutine runFoo4C(ndim) bind(C, name="runFoo")
         use, intrinsic :: iso_c_binding
         implicit none
         integer(c_int32_t) , intent(in) :: ndim
      end subroutine runFoo4C
   end interface

   contains

end module Foo_mod

submodule(Foo_mod) Foo_smod

   contains

      module subroutine runFoo4C(ndim) bind(C, name="runFu")   ! { dg-error "Mismatch in BIND" }
         use, intrinsic :: iso_c_binding                 ! { dg-error "Unexpected USE statement" }
         implicit none                                   ! { dg-error "Unexpected IMPLICIT NONE statement" }
         integer(c_int32_t) , intent(in) :: ndim         ! { dg-error "Unexpected data declaration" }
      end subroutine runFoo4C                            ! { dg-error " Expecting END SUBMODULE" }

end submodule Foo_smod
