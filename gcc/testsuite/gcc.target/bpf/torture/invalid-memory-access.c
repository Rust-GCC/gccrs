// { dg-do run { xfail *-*-* } }
// { dg-options -Wall }

#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

char LICENSE[] SEC("license") = "GPL";

SEC("tracepoint/syscalls/sys_enter_openat")
int bpf_prog(struct trace_event_raw_sys_enter *ctx) {
    int arr[4] = {1, 2, 3, 4};

    // Invalid memory access: out-of-bounds
    int val = arr[5];  // This causes the verifier to fail

    return val;
}

