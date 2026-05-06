/* PR sanitizer/124969 */
/* { dg-do compile } */
/* { dg-options "-fsanitize=undefined" } */

struct S {
  int c;
  void *b __attribute__ ((counted_by (c)));
} a;

void *
foo (void)
{
  return a.b + 10;
}
