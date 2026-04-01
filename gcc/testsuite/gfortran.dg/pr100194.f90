! { dg-do run }
!
! PR fortran/100194
! ICE in gfc_trans_create_temp_array when passing a non-contiguous
! assumed-rank array to a contiguous assumed-rank dummy argument.
!
! Contributed by Martin Diehl <m.diehl@kit.edu>

subroutine s(x)
   real :: x(..)
   call t(x)
contains
   subroutine t(y)
      real, contiguous :: y(..)
   end
end

! The following from the PR, these compiled OK before the patch.
!
! Contributed by G. Steinmetz <gscfq@t-online.de>

subroutine z3(x)
   real, contiguous :: x(..)
   call t(x)
contains
   subroutine t(y)
      real, contiguous :: y(..)
   end
end

subroutine z2(x)
   real, contiguous :: x(..)
   call t(x)
contains
   subroutine t(y)
      real :: y(..)
   end
end

! Runtime follow-up from Paul Thomas <pault@gcc.gnu.org>.

program p
  implicit none
  interface
    subroutine pr100194_runtime(x, res)
      integer :: x(..)
      integer, allocatable :: res(:)
    end subroutine pr100194_runtime
  end interface

  type :: ty
    character(4) :: chr
    integer :: r(2)
  end type ty

  integer :: x(4) = [42, 84, 126, 168]
  type(ty) :: y = ty("abcd", [42, 84])
  integer, allocatable :: z(:)
  integer, allocatable :: u(:, :)

  u = reshape ([x, 4 * x, 16 * x, 64 * x], [4, 4])

  call pr100194_runtime(x, z)
  if (any (z /= [84, 168, 252, 336])) stop 1

  call pr100194_runtime(x(1:4:2), z)
  if (any (z /= [84, 252])) stop 2

  call pr100194_runtime(x([1, 3, 4, 2]), z)
  if (any (z /= [84, 252, 336, 168])) stop 3

  call pr100194_runtime(y%r, z)
  if (any (z /= [84, 168])) stop 4

  call pr100194_runtime(u(1, :), z)
  if (any (z /= [84, 336, 1344, 5376])) stop 5

  call pr100194_runtime(u(:, 1), z)
  if (any (z /= [84, 168, 252, 336])) stop 6
end program p

subroutine pr100194_runtime(x, res)
  integer :: x(..)
  integer, allocatable :: res(:)

  call t(x)

contains

  subroutine t(y)
    integer, contiguous :: y(..)

    select rank (y)
      rank (1)
        res = 2 * y
    end select
  end subroutine t
end subroutine pr100194_runtime
