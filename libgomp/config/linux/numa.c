/* Copyright (C) 2026 Free Software Foundation, Inc.
   Contributed by Tobias Burnus <tburnus@baylibre.com>

   This file is part of the GNU Offloading and Multi Processing Library
   (libgomp).

   Libgomp is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   Libgomp is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

/* Linux specific version of internally used NUMA information routines.  */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#include "libgomp.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>


static int num_numa_nodes = 0;
static int *numa_distances = NULL;

int
gomp_get_current_numa_node ()
{
  int node;
  syscall (SYS_getcpu, NULL /* cpu */, &node, NULL /* no longer used */);
  return node;
}

int
gomp_get_numa_distance (int node1, int node2)
{
  if (node1 < 0 || node2 < 0 || num_numa_nodes < 0)
    return -1;

  if (numa_distances == NULL)
    {
      num_numa_nodes = -1;
      DIR *dir = opendir ("/sys/devices/system/node");
      if (!dir)
	return -1;
      struct dirent *dp;
      int cnt = 0;
      errno = 0;
      while ((dp = readdir(dir)) != NULL)
	if (strncmp ("node", dp->d_name, 4 /* strlen ("node") */) == 0)
	  cnt++;
	else if (errno)
	  {
	    closedir (dir);
	    return -1;
	  }
      closedir (dir);
      numa_distances = (int *) gomp_malloc (sizeof (int) * cnt * cnt);

      constexpr int len = sizeof ("/sys/devices/system/node/node12345/"
				  "distance");
      char filename[len];

      for (int i = 0; i < cnt; i++)
	{
	  if (len < snprintf (filename, sizeof (filename),
			      "/sys/devices/system/node/node%d/distance", i))
	    return -1;
	  int distance = -1;
	  FILE *in = fopen (filename, "r");
	  for (int j = 0; j < cnt; j++)
	    {
	      fscanf (in, "%d", &distance);
	      if (distance == -1)
		{
		  fclose (in);
		  free (numa_distances);
		  return -1;
		}
	      numa_distances[i * cnt + j] = distance;
	    }
	  fclose (in);
	}
      num_numa_nodes = cnt;
    }
  return numa_distances[node1 * num_numa_nodes + node2];
}
