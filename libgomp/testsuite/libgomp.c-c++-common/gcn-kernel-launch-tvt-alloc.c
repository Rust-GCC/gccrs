/* { dg-do run } */
/* { dg-require-effective-target offload_device_gcn } */

/* Test that the no-merge case still works.  */

#define INHIBIT_KERNARGS_MERGE
#include "./gcn-kernel-launch-no-tvt-alloc.c"

/* See commentary in <gcn-kernel-launch-no-tvt-alloc.c>.

   Ignore ===... marker and everything before it.
     { dg-output {^.*================[\r\n]+} }
   We expect at least "GCN debug: Allocating \d+ bytes..."
     { dg-output {.*[\r\n]+GCN debug: Allocating \d+ bytes.*} }
   There should be nothing left.
     { dg-output {$} }  */
