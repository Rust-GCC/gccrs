program main
#ifndef USE_OMP_HEADER
  use omp_lib
#endif
  implicit none (type, external)

#ifdef USE_OMP_HEADER
  include "omp_lib.h"
#endif

  integer :: dev, dim

  ! Supported active teams/league dimension - the assumed result for either,
  ! once implemented, is:
  ! - Host: native == 1, but supporting (emulating) any dimension
  ! - Non-host: native == 3 and not supporting higher dimension,
  !   i.e. expected that no one implements this.

  ! Always true: All devices support at least one dimension,
  ! with native_support set to true or to false.

  if (omp_get_supported_active_team_dims (omp_initial_device, .true.) < 1) &
    stop 1
  if (omp_get_supported_active_team_dims (omp_initial_device, .false.) < 1) &
    stop 2
  if (omp_get_supported_active_league_dims (omp_initial_device, .true.) < 1) &
    stop 3
  if (omp_get_supported_active_league_dims (omp_initial_device, .false.) < 1) &
    stop 4
  do dev = 0, omp_get_num_devices () - 1
    if (omp_get_supported_active_team_dims (dev, .true.) < 1) &
      stop 5
    if (omp_get_supported_active_team_dims (dev, .false.) < 1) &
      stop 6
    if (omp_get_supported_active_league_dims (dev, .true.) < 1) &
      stop 7
    if (omp_get_supported_active_league_dims (dev, .false.) < 1) &
      stop 8
  end do

  ! For each supported dimension, expect that it supports more than
  ! one team and thread. - And for each unsupported dimension, expect
  ! that exactly one thread and team is reported as supported.

  do dev = 0, omp_get_num_devices () - 1
  block
    integer :: max_dims, test_size, test_end
    max_dims = omp_get_supported_active_team_dims (dev, .false.)
    test_size = (max_dims == huge(max_dims) ? 64 : max_dims)
    do dim = 1, test_size
      if (omp_get_supported_teams_dim (dev, dim) < 1) &
        stop 9
    end do
    test_end = (huge (test_size) - 12 > test_size ? test_size + 12 : huge(test_size))
    if (max_dims < huge(max_dims)) then
      do dim = test_size + 1, test_end - 1
        if (omp_get_supported_teams_dim (dev, dim) /= 1) &
          stop 10
      end do
    end if

    max_dims = omp_get_supported_active_league_dims (dev, .false.)
    test_size = (max_dims == huge(max_dims) ? 64 : max_dims)
    do dim = 1, test_size
      if (omp_get_supported_threads_dim (dev, dim) < 1) &
        stop 11
    end do
    test_end = (huge(test_size) - 12 > test_size ? test_size + 12 : huge(test_size))
    if (max_dims < huge(max_dims)) then
      do dim = test_size + 1, test_end - 1
        if (omp_get_supported_threads_dim (dev, dim) /= 1) &
          stop 12
      end do
    else
      do dim = huge (max_dims) - 12, huge (max_dims) - 1
        if (omp_get_supported_threads_dim (dev, dim) /= 1) &
          stop 13
      end do
    end if
  end block
  end do

  ! Implementation specific to GCC:
  ! Currently, exactly one dimension is supported on the host and
  ! on the device, both natively and emulated.

  if (omp_get_supported_active_team_dims (omp_initial_device, .true.) /= 1) &
    stop 14
  if (omp_get_supported_active_team_dims (omp_initial_device, .false.) /= 1) &
    stop 15
  if (omp_get_supported_active_league_dims (omp_initial_device, .true.) /= 1) &
    stop 16
  if (omp_get_supported_active_league_dims (omp_initial_device, .false.) /= 1) &
    stop 17
  do dev = 0, omp_get_num_devices () - 1
    if (omp_get_supported_active_team_dims (dev, .true.) /= 1) &
      stop 18
    if (omp_get_supported_active_team_dims (dev, .false.) /= 1) &
      stop 19
    if (omp_get_supported_active_league_dims (dev, .true.) /= 1) &
      stop 20
    if (omp_get_supported_active_league_dims (dev, .false.) /= 1) &
      stop 21
  end do

  ! TODO: Once implemented, add a check for 'dim(N)', N > 1 below.


  ! The following should always pass as only a single spacial dimension
  ! is used.

  do dev = omp_initial_device, omp_get_num_devices () - 1
    !$omp target teams device(dev) num_teams(10) thread_limit(8)
      if (omp_get_num_teams () /= 10  &
          .or. omp_get_num_teams_dim (0) /= 10) &
        stop 22
      if (omp_get_team_num () /= omp_get_team_num_dim (0) &
          .or. omp_get_team_num_dim (0) < 0 &
          .or. omp_get_team_num_dim (0) >= 10) &
        stop 23
      do dim = 1, 12
        if (omp_get_num_teams_dim (dim) /= 1 &
            .or. omp_get_team_num_dim (dim) /= 0) &
          stop 24
      end do
      !$omp parallel num_threads(8)
            ! FIXME: Assumes 'num_threads(strict:', add once modifier is supported
        if (omp_get_thread_limit_dim (0) /= 8) &
          stop 25
        do dim = 1, 12
          if (omp_get_thread_limit_dim (dim) /= 1) &
            stop 26
        end do
        if (omp_get_num_threads () /= 8 &
            .or. omp_get_num_threads_dim (0) /= 8) &
          stop 27
        if (omp_get_team_num () /= omp_get_team_num_dim (0) &
            .or. omp_get_thread_num_dim (0) < 0 &
            .or. omp_get_thread_num_dim (0) >= 8) &
          stop 28
        do dim = 1, 12
          if (omp_get_num_threads_dim (dim) /= 1 &
              .or. omp_get_thread_num_dim (dim) /= 0) &
            stop 29
        end do
      !$omp end parallel
    !$omp end target teams
  end do
end program
