/* Verify that _m_prefetchw does not produce -Wcast-qual warnings when
   included as a system header.  The cast (const void *) drops volatile
   from volatile const void *, which would trigger -Wcast-qual if not
   in a system header.  */
/* { dg-do compile } */
/* { dg-options "-mprfchw -O2 -Wcast-qual" } */

#include <x86intrin.h>

void
prefetchw_cast_qual_test (volatile void *p)
{
    _m_prefetchw (p);
}

/* { dg-bogus "cast discards .volatile. qualifier" "" { target *-*-* } 0 } */
