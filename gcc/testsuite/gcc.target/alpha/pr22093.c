/* { dg-do compile } */
/* { dg-options "" } */

struct shared_ptr_struct
{
  unsigned long phase : 48;
  unsigned thread : 16;
  void *addr;
} x;

void foo (void)
{
   x.thread = 2;
}
