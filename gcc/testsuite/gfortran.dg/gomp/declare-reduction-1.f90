! { dg-do compile }

module one
  implicit none
  type t
    integer :: x
  end type t
end module

module two
  use one
  implicit none
  !$omp declare reduction(+ : t : omp_out%x = omp_out%x + omp_in%x) initializer (omp_priv%x = 0)
end module

module three
  use one
  implicit none
  !$omp declare reduction(+ : t : omp_out%x = omp_out%x + omp_in%x) initializer (omp_priv%x = 0)
end module

subroutine sub2
  use two    ! { dg-note "Previous !.OMP DECLARE REDUCTION from module 'two'" }
  use three  ! { dg-error "Ambiguous !.OMP DECLARE REDUCTION 'operator \\+' for type 'TYPE\\(t\\)' from module 'three' at .1." }
  implicit none
  type(t) :: var(3), sum
  integer :: i

  var(:)%x = [1,2,3]
  sum%x = 0

  !$omp parallel do reduction(+: sum)
  do i = 1, 3
    sum%x = sum%x + var(i)%x 
  end do
end

call sub2
end
