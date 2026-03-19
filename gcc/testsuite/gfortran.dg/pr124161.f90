! { dg-do compile }
!
! Test the fix for the regression caused by r16-669-gd31ab498b12ebb - See line 59.
!
! Contributed by Zaak Beekman  <zbeekman@gmail.com>
!
module inner_m
    implicit none
    type :: dt_t
        integer, allocatable :: val
    end type
    type, abstract :: inner_t
    contains
        procedure(summary_i), deferred :: summary
    end type
    abstract interface
        function summary_i(self)
            import :: inner_t, dt_t
            class(inner_t), intent(in) :: self
            type(dt_t) :: summary_i
        end function
    end interface
end module inner_m

module concrete_m
    use inner_m, only: inner_t, dt_t
    implicit none
    type, extends(inner_t) :: concrete_t
    contains
        procedure :: summary
    end type
contains
    function summary(self)
        class(concrete_t), intent(in) :: self
        type(dt_t) :: summary
    end function
end module concrete_m

module outer_m
    use inner_m, only: inner_t, dt_t
    use concrete_m, only: concrete_t
    implicit none
    type :: outer_t
        class(inner_t), allocatable :: component
    contains
        procedure :: summary
    end type
    interface
        module function summary(self) result(res)
            class(outer_t), intent(in) :: self
            type(dt_t) :: res
        end function
    end interface
end module outer_m

submodule(outer_m) outer_impl
contains
    module procedure summary
        res = self%component%summary()  ! ICE in trans_scalar_assign here
    end procedure
end submodule outer_impl
