/* { dg-do compile } */
/* { dg-options "-O2" } */

char *
foo ()
{
  return (char *) (__UINTPTR_TYPE__) foo + 7483647;
}

/* { dg-final { scan-assembler-not "\\.(word|xword)\tfoo" } } */
