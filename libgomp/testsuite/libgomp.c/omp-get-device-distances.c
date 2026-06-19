#include <omp.h>

int
main ()
{
  int dev, dist;
  int dev2, dist2;

  // GCC specific: invalid numbers return -1
  dev = omp_invalid_device; dist = -99;
  omp_get_device_distances (1, &dev, &dist);
  if (dev != omp_invalid_device || dist != -1)
    __builtin_abort ();

  dev = -99; dist = -99;
  omp_get_device_distances (1, &dev, &dist);
  if (dev != -99 || dist != -1)
    __builtin_abort ();

  dev = omp_get_num_devices () + 1; dist = -99;
  omp_get_device_distances (1, &dev, &dist);
  if (dev != omp_get_num_devices () + 1 || dist != -1)
    __builtin_abort ();

  // The following two need to yield idential results
  dev = omp_default_device; dist = -99;
  omp_get_device_distances (1, &dev, &dist);
  if (dev != omp_default_device || dist < 0)
    __builtin_abort ();

  dev2 = omp_get_default_device (); dist2 = -99;
  omp_get_device_distances (1, &dev2, &dist2);
  if (dev2 != omp_get_default_device () || dist2 != dist)
    __builtin_abort ();

  // The following two need to yield idential results
  dev = omp_initial_device; dist = -99;
  omp_get_device_distances (1, &dev, &dist);
  if (dev != omp_initial_device || dist != 0)
    __builtin_abort ();

  dev2 = omp_get_num_devices (); dist2 = -99;
  omp_get_device_distances (1, &dev2, &dist2);
  if (dev2 != omp_get_num_devices () || dist2 != 0)
    __builtin_abort ();

  for (int i = omp_initial_device; i <= omp_get_num_devices (); i++)
    {
      dev = i; dist = -99;
      omp_get_device_distances (1, &dev, &dist);
      if (dev != i)
	__builtin_abort ();
      // Host == 0 per definition
      // GCC specific: All others (unknown or not) should have > 0
      // the spec only requires >= 0
      if (i == omp_initial_device || i == omp_get_num_devices ())
	{
	  if (dist != 0)
	    __builtin_abort ();
	}
      else
	{
	  if (dist <= 0)
	    __builtin_abort ();
	}
     }

  int *devs, *dists;
  int size = 1 + 1 + omp_get_num_devices ();
  devs = (int *) __builtin_malloc (sizeof (int) * size * size);
  dists = (int *) __builtin_malloc (sizeof (int) * size * size);
  for (int i = 0; i < size; i++)
    {
      int dev = i - 1; // -1 (initial), 0, ... num_dev
      devs[i] = dev;
      dists[i] = -99;
    }

  omp_get_device_distances (size, devs, dists);

  for (int i = 0; i < size; i++)
    {
      int dev = i - 1; // -1 (initial), 0, ... num_dev
      if (devs[i] != dev)
	__builtin_abort ();
      // Host == 0 per definition
      // GCC specific: All others (unknown or not) should have > 0
      // the spec only requires >= 0
      if (devs[i] == omp_initial_device || devs[i] == omp_get_num_devices ())
	{
	  if (dists[i] != 0)
	    __builtin_abort ();
	}
      else
	{
	  if (dists[i] <= 0)
	    __builtin_abort ();
	}
      __builtin_printf ("Device%3d: distance %d\n", dev, dists[i]);
    }
}
