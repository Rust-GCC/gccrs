! { dg-do run }
! { dg-options "-O2 -fcheck=bounds -fno-automatic" }
! PR fortran/106546 - FE temporaries have to be automatic
! Original test case by Solomon Gibbs

subroutine do_multiply()
  implicit none
  
  integer :: rank
  real(8) :: draw

  real(8) :: x1(6), x2(6)
  real(8), allocatable :: K(:,:), J(:,:), z(:)

  character(len=300) :: out
  ! Randomly select rank of operation 0 ... 6
  call random_number(draw)
  if (draw .lt. 1/7.0d0) then
     rank = 0
  elseif (draw .lt. 2/7.0d0) then
     rank = 1
  elseif (draw .lt. 3/7.0d0) then
     rank = 2
  elseif (draw .lt. 4/7.0d0) then
     rank = 3
  elseif (draw .lt. 5/7.0d0) then
     rank = 4
  elseif (draw .lt. 6/7.0d0) then
     rank = 5
  else
     rank = 6
  endif

  allocate(K(rank, 6))
  allocate(J(rank, rank))
  allocate(z(rank))

  call random_number(x1)
  call random_number(x2)
  call random_number(K)
  call random_number(J)
  call random_number(z)

  ! multiply allocatables using array temporaries
  ! Problem occurs here
  z = matmul(J, matmul(K, x2 - x1))

  ! Output z to prevent optimizer elimination
  write (out,*) rank," ",z

  deallocate(z, J, K)
  
end subroutine do_multiply
  
program bogus
  implicit none

  integer :: i
  do i=1,100
     call do_multiply()
  end do
  
end program bogus
