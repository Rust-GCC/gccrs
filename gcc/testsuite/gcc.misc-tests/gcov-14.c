/* Test gcov extern inline.  */

/* { dg-do run { target native } } */
/* { dg-options "-O2 -fprofile-arcs -ftest-coverage -fgnu89-inline" } */
/* The following line arranges that Darwin has behavior like elf weak import.  */
/* { dg-additional-options "-Wl,-U,_Foo" { target *-*-darwin* }  } */
/* { dg-require-weak "" } */
/* { dg-skip-if "undefined weak not supported" { { hppa*-*-hpux* } && { ! lp64 } } } */
/* { dg-skip-if "undefined weak not supported" { powerpc-ibm-aix* } } */

extern int __attribute__ ((weak)) Foo ();

extern __inline int Foo ()
{
  return 0; /* count(-) */
}

int (* __attribute__ ((noinline)) Bar ()) ()
{
  return Foo; /* count(1) */
}

int main ()
{
  return Bar () != 0; /* count(1) */
}

/* { dg-final { run-gcov { -a gcov-14.c } } } */
