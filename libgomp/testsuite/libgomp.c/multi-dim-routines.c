#include <omp.h>

int
main ()
{
  /* Supported active teams/league dimension - the assumed result for either,
     once implemented, is:
     - Host: native == 1, but supporting (emulating) any dimension
     - Non-host: native == 3 and not supporting higher dimension,
       i.e. expected that no one implements this.  */

  /* Always true: All devices support at least one dimension,
     with native_support set to true or to false.  */

  if (omp_get_supported_active_team_dims (omp_initial_device, true) < 1)
    __builtin_abort ();
  if (omp_get_supported_active_team_dims (omp_initial_device, false) < 1)
    __builtin_abort ();
  if (omp_get_supported_active_league_dims (omp_initial_device, true) < 1)
    __builtin_abort ();
  if (omp_get_supported_active_league_dims (omp_initial_device, false) < 1)
    __builtin_abort ();
  for (int dev = 0; dev < omp_get_num_devices (); dev++)
    {
      if (omp_get_supported_active_team_dims (dev, true) < 1)
      __builtin_abort ();
      if (omp_get_supported_active_team_dims (dev, false) < 1)
      __builtin_abort ();
      if (omp_get_supported_active_league_dims (dev, true) < 1)
      __builtin_abort ();
      if (omp_get_supported_active_league_dims (dev, false) < 1)
      __builtin_abort ();
    }

  /* For each supported dimension, expect that it supports more than
     one team and thread. - And for each unsupported dimension, expect
     that exactly one thread and team is reported as supported.  */

  for (int dev = 0; dev < omp_get_num_devices (); dev++)
    {
      int max_dims = omp_get_supported_active_team_dims (dev, false);
      int test_size = max_dims == __INT_MAX__ ? 64 : max_dims;
      for (int dim = 1; dim <= test_size; dim++)
	if (omp_get_supported_teams_dim (dev, dim) < 1)
	  __builtin_abort ();
      int test_end = __INT_MAX__ - 12 > test_size ? test_size + 12 : __INT_MAX__;
      if (max_dims < __INT_MAX__)
	for (int dim = test_size + 1; dim < test_end; dim++)
	  if (omp_get_supported_teams_dim (dev, dim) != 1)
	    __builtin_abort ();

      max_dims = omp_get_supported_active_league_dims (dev, false);
      test_size = max_dims == __INT_MAX__ ? 64 : max_dims;
      for (int dim = 1; dim <= test_size; dim++)
	if (omp_get_supported_threads_dim (dev, dim) < 1)
	  __builtin_abort ();
      test_end = __INT_MAX__ - 12 > test_size ? test_size + 12 : __INT_MAX__;
      if (max_dims < __INT_MAX__)
	for (int dim = test_size + 1; dim < test_end; dim++)
	  if (omp_get_supported_threads_dim (dev, dim) != 1)
	    __builtin_abort ();
      if (max_dims == __INT_MAX__)
	for (int dim = __INT_MAX__ - 12; dim < __INT_MAX__; dim++)
	  if (omp_get_supported_threads_dim (dev, dim) != 1)
	    __builtin_abort ();
    }

  /* Implementation specific to GCC:
     Currently, exactly one dimension is supported on the host and
     on the device, both natively and emulated.  */ 

  if (omp_get_supported_active_team_dims (omp_initial_device, true) != 1)
    __builtin_abort ();
  if (omp_get_supported_active_team_dims (omp_initial_device, false) != 1)
    __builtin_abort ();
  if (omp_get_supported_active_league_dims (omp_initial_device, true) != 1)
    __builtin_abort ();
  if (omp_get_supported_active_league_dims (omp_initial_device, false) != 1)
    __builtin_abort ();
  for (int dev = 0; dev < omp_get_num_devices (); dev++)
    {
      if (omp_get_supported_active_team_dims (dev, true) != 1)
      __builtin_abort ();
      if (omp_get_supported_active_team_dims (dev, false) != 1)
      __builtin_abort ();
      if (omp_get_supported_active_league_dims (dev, true) != 1)
      __builtin_abort ();
      if (omp_get_supported_active_league_dims (dev, false) != 1)
      __builtin_abort ();
    }

  /* TODO: Once implemented, add a check for 'dim(N)', N > 1 below.  */


  /* The following should always pass as only a single spacial dimension
     is used.  */

  for (int dev = omp_initial_device; dev < omp_get_num_devices (); dev++)
    {
      #pragma omp target teams device(dev) num_teams(10) thread_limit(8)
	{
	  if (omp_get_num_teams () != 10
	      || omp_get_num_teams_dim (0) != 10)
	    __builtin_abort ();
	  if (omp_get_team_num () != omp_get_team_num_dim (0)
	      || omp_get_team_num_dim (0) < 0
	      || omp_get_team_num_dim (0) >= 10)
	    __builtin_abort ();
	  for (int dim = 1; dim <= 12; dim++)
	    if (omp_get_num_teams_dim (dim) != 1
		|| omp_get_team_num_dim (dim) != 0)
	      __builtin_abort ();
          #pragma omp parallel num_threads(8)
          // FIXME: Assumes 'num_threads(strict:', add once modifier is supported
	  {
	    if (omp_get_thread_limit_dim (0) != 8)
		__builtin_abort ();
	    for (int dim = 1; dim <= 12; dim++)
	      if (omp_get_thread_limit_dim (dim) != 1)
		__builtin_abort ();
	    if (omp_get_num_threads () != 8
		|| omp_get_num_threads_dim (0) != 8)
	      __builtin_abort ();
	    if (omp_get_team_num () != omp_get_team_num_dim (0)
		|| omp_get_thread_num_dim (0) < 0
		|| omp_get_thread_num_dim (0) >= 8)
	      __builtin_abort ();
	    for (int dim = 1; dim <= 12; dim++)
	      if (omp_get_num_threads_dim (dim) != 1
		  || omp_get_thread_num_dim (dim) != 0)
		__builtin_abort ();
	  }
        }
    }
}
