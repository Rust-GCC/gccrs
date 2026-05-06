/* PR sanitizer/124969 */
/* { dg-do compile } */
/* { dg-options "-fsanitize=undefined" } */

struct T;
struct S {
  int c;
  struct T *b __attribute__ ((counted_by (c)));
} a;

void *
foo ()
{
  return a.b + 10;	/* { dg-error "invalid use of undefined type 'struct T'" } */
}
