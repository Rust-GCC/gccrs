/* { dg-do run } */
/* { dg-require-effective-target offload_device_gcn } */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int
main ()
{
  setenv ("GCN_DEBUG", "1", true);
#ifdef INHIBIT_KERNARGS_MERGE
  /* See gcn-kernel-launch-tvt-alloc.c  */
  setenv ("GCN_INHIBIT_KERNARGS_TVT_MERGE", "1", true);
#endif

  int i[1] = { 0 };

#pragma omp target data map(tofrom: i[0:1])
  {
    fprintf (stderr, "================\n");

#pragma omp target
    { i[0] = 1; }
  }

  assert (i[0] == 1);
}

/* Here, we want to ensure that we have no allocations after the point
   delimited by ===...

   Past that point, the only data to map onto the device is the target
   variable table, which should be passed as kernel arguments.  The GCN plugin
   currently does not log allocating those.  We rely on that here.

   So, dg-output lets us match the entire output with a regex.  Multiple
   dg-output invocations will have their regexes concatenated in order.  The
   following is that regex, broken down by function:

   Ignore ===... marker and everything before it.
     { dg-output {^.*================[\r\n]+} }
   Then, each further line is either...
     { dg-output {((} }
   ... a line not starting with "GCN debug: "...
     { dg-output {(?!GCN debug:)[^\r\n]+} }
   ... or a "GCN debug: ..." line that is not an allocation:
     { dg-output {|GCN debug: (?!Allocating )[^\r\n]*} }
   ... followed by a line terminator, of course.
     { dg-output {)[\r\n]+)*} }
   There should be nothing left.
     { dg-output {$} }  */
