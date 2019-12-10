/* { dg-do compile } */

#include <stdint.h>

void
foo ()
{
  int ret;
  void *ctx, *map;
  uint64_t flags;
  void *data;
  uint64_t size;

  ret = __builtin_bpf_helper_perf_event_output (ctx, map, flags, data, size);
}

/* { dg-final { scan-assembler "call\t25" } } */
