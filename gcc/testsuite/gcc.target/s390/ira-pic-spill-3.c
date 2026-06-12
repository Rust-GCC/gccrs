/* { dg-do compile } */
/* { dg-options "-O2 -march=z14 -fPIC" } */
/* { dg-final { scan-assembler-not {lay\t%r15,-[0-9]+\(%r15\)} } } */

/* Ensure no spills and ultimately that no stack frame is allocated.  */

int _mode;
struct _IO_FILE_plus {
  int vtable;
};
enum { IO_FILE_JUMPS_MAYBE_MMAP, IO_WFILE_JUMPS_MAYBE_MMAP };
extern int __io_vtables[] __attribute__((visibility("hidden")));
void __fopen_maybe_mmap_fp() {
  if (_mode)
    *(__typeof__(&(struct _IO_FILE_plus){}.vtable) *)__fopen_maybe_mmap_fp =
        &__io_vtables[IO_FILE_JUMPS_MAYBE_MMAP];
  else
    *(__typeof__(&(struct _IO_FILE_plus){}.vtable) *)__fopen_maybe_mmap_fp =
        &__io_vtables[IO_WFILE_JUMPS_MAYBE_MMAP];
}
