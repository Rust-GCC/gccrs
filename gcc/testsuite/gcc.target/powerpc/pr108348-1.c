/* { dg-require-effective-target powerpc_altivec_ok } */
/* If the default cpu type is power10 or later, type __vector_quad is
   supported.  To keep the test point available all the time, this case
   specifies -mdejagnu-cpu=power9 here.  This needs -mabi=no-altivec
   to do the copying for pass-by-reference function argument on 32 bit
   environment.  */
/* { dg-options "-mdejagnu-cpu=power9 -mabi=no-altivec" } */

/* Verify there is no ICE on 32 bit and don't check the error messages
   on unsupported type since they could be fragile and are not test
   points of this case.  */

/* { dg-excess-errors "pr108348-1" } */

extern void bar (__vector_quad v);

void
foo (void)
{
  __vector_quad v;
  bar (v);
}

