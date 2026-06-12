! Check that type renaming via module use is properly handled.

module first
  implicit none
  type t
    integer :: x = 0
  end type t
  !$omp declare reduction(+: t : omp_out%x = omp_out%x + omp_in%x)
end module

module second
  implicit none
  type t
    integer :: y = 0
  end type t
  !$omp declare reduction(+: t : omp_out%y = omp_out%y + omp_in%y)
end module

subroutine sub2
 use first, t1 => t
 use second, t2 => t
 implicit none
  type(t1) :: var(5), sum1
  type(t2) :: var2(3), sum2
  integer :: i

  var%x = [(2*i, i = 1,5)]
  var2%y = [(5*i+1, i = 1,3)]
  sum1%x = 0
  sum2%y = 0

  !$omp parallel do reduction(+:sum1)
  do i = 1, size(var)
    sum1%x = sum1%x + var(i)%x
  end do 

  !$omp parallel do reduction(+:sum2)
  do i = 1, size(var2)
    sum2%y = sum2%y + var2(i)%y
  end do 
  
  if (sum1%x /= sum(var%x)) stop 1
  if (sum2%y /= sum(var2%y)) stop 2
end

call sub2
end
