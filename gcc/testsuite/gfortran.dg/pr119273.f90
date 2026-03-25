! { dg-do compile }
! { dg-options "-fdump-tree-original -fcheck=bounds" }
!
! Test the fix forPR119273, which caused false bounds check errors.
!
! Contributed by Jingwei Xing  <xingjingwei666@gmail.com>
!
module test_module
    type :: test_type_father
        integer :: val
    contains
        procedure :: test_function3
    end type test_type_father

    type, extends(test_type_father) :: test_type_child
    contains
        procedure :: test_function1
    end type test_type_child

    integer :: ii

    contains
    function test_function1(a) result(out)
        class(test_type_child), intent(in) :: a
        integer :: out
        ii = a%val  ! Make this function implicitly impure.
        out = a%val
    end function
    pure function test_function2(a) result(out)
        type(test_type_child), intent(in) :: a
        integer :: out
        out = a%val
    end function
    pure function test_function3(a) result(out)
        class(test_type_father), intent(in) :: a
        integer :: out
        out = a%val
    end function
end module

program test
    use test_module
    implicit none
    type(test_type_child) :: a
    type(test_type_father) :: b
    class(test_type_child), allocatable :: c
    integer, target :: temp_int(1,1)
    integer, pointer :: d(:)
    a%val = 0
    associate(temp => temp_int(:,a%test_function1() + 1)) ! { dg-warning "is not implicitly pure" }
    end associate
    a%val = 1
    associate(temp => temp_int(:,test_function2(a)))
    end associate
    b%val = 1
    associate(temp => temp_int(:,b%test_function3()))
    end associate
    associate(temp => temp_int(:,test_function3(b)))
    end associate
    allocate (c, source = a)
    associate(temp => temp_int(:,c%test_function1())) ! { dg-warning "is not implicitly pure" }
    end associate

! Demonstrate that this bug was not specific to the ASSOCIATE statement but,
! rather to it being a pointer assignment, when the selector is a variable.
    d => temp_int(:,a%test_function1()) ! { dg-warning "is not implicitly pure" }

end program test
! Test for the missing vptr assignments to the class actual temporaries.
! { dg-final { scan-tree-dump-times "class..._vptr =" 8 "original" } }
