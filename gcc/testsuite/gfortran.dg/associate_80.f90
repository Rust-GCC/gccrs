! { dg-do run }
!
! Test ASSOCIATE selectors that are type-bound user-defined operators (UDOs),
! including inherited operators and nested expressions such as
! .div. (.grad. x).  PR fortran/125515
!
module associate_80_m
  implicit none

  type :: scalar_t
    real :: val
  contains
    generic :: operator(.grad.) => do_grad
    procedure, private :: do_grad
    generic :: operator(.sq.)   => do_sq
    procedure, private :: do_sq
    generic :: get => get_val
    procedure :: get_val
  end type

  type, extends(scalar_t) :: vector_t
  contains
    generic :: operator(.div.) => do_div
    procedure, private :: do_div
  end type

contains

  ! .grad. on scalar_t -> vector_t
  pure function do_grad (self) result (r)
    class(scalar_t), intent(in) :: self
    type(vector_t) :: r
    r%val = self%val * 2.0
  end function

  ! .sq. on scalar_t -> scalar_t
  pure function do_sq (self) result (r)
    class(scalar_t), intent(in) :: self
    type(scalar_t) :: r
    r%val = self%val * self%val
  end function

  ! .div. on vector_t -> scalar_t
  pure function do_div (self) result (r)
    class(vector_t), intent(in) :: self
    type(scalar_t) :: r
    r%val = self%val / 2.0
  end function

  pure function get_val (self) result (r)
    class(scalar_t), intent(in) :: self
    real :: r
    r = self%val
  end function

end module associate_80_m

program associate_80
  use associate_80_m
  implicit none

  type(scalar_t) :: s
  s%val = 3.0

  ! Case 1: direct type-bound UDO as ASSOCIATE selector
  associate (g => .grad. s)
    if (abs (g%val - 6.0) > 1.0e-6) stop 1
    if (abs (g%get () - 6.0) > 1.0e-6) stop 2
  end associate

  ! Case 2: inherited UDO (.sq. defined on scalar_t, used via scalar_t)
  associate (q => .sq. s)
    if (abs (q%val - 9.0) > 1.0e-6) stop 3
  end associate

  ! Case 3: nested UDOs — .div. (.grad. s)
  ! .grad. s gives vector_t with val=6, .div. gives scalar_t with val=3
  associate (r => .div. (.grad. s))
    if (abs (r%val - 3.0) > 1.0e-6) stop 4
    if (abs (r%get () - 3.0) > 1.0e-6) stop 5
  end associate

end program associate_80
