/* { dg-do compile } */
/* { dg-options "-O2 -march=z14 -fPIC" } */
/* { dg-final { scan-assembler-not {lay\t%r15,-[0-9]+\(%r15\)} } } */

/* Ensure no spills and ultimately that no stack frame is allocated.  */

typedef long int __time_t;
typedef long int __syscall_slong_t;
struct timespec
{
  __time_t tv_sec;
  __syscall_slong_t tv_nsec;
};

struct timespec
support_timespec_normalize (struct timespec time)
{
  struct timespec norm;
  norm.tv_sec = time.tv_nsec ? 1073741824 : 65536;
  return norm;
}
