// { dg-do run  }
// { dg-options -Wall }

#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

char LICENSE[] SEC("license") = "GPL";

int example_pid = 0;

SEC("tracepoint/syscalls/sys_enter_openat")
int handle_openat(struct trace_event_raw_sys_enter *ctx)
{
	int pid = bpf_get_current_pid_tgid() >> 32;
	char filename[256]; // filename buffer
	bpf_probe_read_user(&filename, sizeof(filename), (void *)ctx->args[1]);
	bpf_printk("sys_enter_openat() called from PID %d for file: %s\n", pid, filename);

	return 0;
}

/* { dg-output "BPF programs successfully loaded" } */
