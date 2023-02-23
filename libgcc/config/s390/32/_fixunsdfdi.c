/* Definitions of target machine for GNU compiler, for IBM S/390
   Copyright (C) 1999-2023 Free Software Foundation, Inc.
   Contributed by Hartmut Penner (hpenner@de.ibm.com) and
                  Ulrich Weigand (uweigand@de.ibm.com).

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#ifndef __s390x__

#define EXPD(fp)	(((fp.l.upper) >> 20) & 0x7FF)
#define EXPONENT_BIAS	1023
#define MANTISSA_BITS   52
#define PRECISION       (MANTISSA_BITS + 1)
#define SIGNBIT		0x80000000
#define SIGN(fp)	((fp.l.upper) & SIGNBIT)
#define MANTD_LL(fp)	((fp.ll & (HIDDEND_LL-1)) | HIDDEND_LL)
#define FRACD_LL(fp)	(fp.ll & (HIDDEND_LL-1))
#define HIDDEND_LL	((UDItype_x)1 << 52)

typedef int DItype_x __attribute__ ((mode (DI)));
typedef unsigned int UDItype_x __attribute__ ((mode (DI)));
typedef int SItype_x __attribute__ ((mode (SI)));
typedef unsigned int USItype_x __attribute__ ((mode (SI)));

union double_long {
    double d;
    struct {
      SItype_x upper;
      USItype_x lower;
    } l;
    UDItype_x ll;
};

static __inline__ void
fexceptdiv (float d, float e)
{
  __asm__ __volatile__ ("debr %0,%1" : : "f" (d), "f" (e) );
}

UDItype_x __fixunsdfdi (double a1);

/* convert double to unsigned int */
UDItype_x
__fixunsdfdi (double a1)
{
    register union double_long dl1;
    register int exp;
    register UDItype_x l;

    dl1.d = a1;

    /* +/- 0, denormalized  */
    if (!EXPD (dl1))
      return 0;

    /* Negative.  */
    if (SIGN (dl1))
      { 
	/* Value is <= -1.0
	   C99 Annex F.4 requires an "invalid" exception to be thrown.  */
	if (EXPD (dl1) >= EXPONENT_BIAS)
	  fexceptdiv (0.0, 0.0);
	return 0;
      }

    exp = EXPD (dl1) - EXPONENT_BIAS - MANTISSA_BITS;

    /* number < 1 */

    if (exp < -PRECISION)
      return 0;

    /* NaN */

    if ((EXPD(dl1) == 0x7ff) && (FRACD_LL(dl1) != 0)) /* NaN */
      {
	/* C99 Annex F.4 requires an "invalid" exception to be thrown.  */
	fexceptdiv (0.0, 0.0);
	return 0x0ULL;
      }

    /* Number big number & + inf */

    if (exp >= 12)
      {
	/* C99 Annex F.4 requires an "invalid" exception to be thrown.  */
	fexceptdiv (0.0, 0.0);
	return 0xFFFFFFFFFFFFFFFFULL;
      }

    l = MANTD_LL(dl1);

    /* shift down until exp < 12 or l = 0 */
    if (exp > 0)
      l <<= exp;
    else
      l >>= -exp;

    return l;
}
#endif /* !__s390x__ */
