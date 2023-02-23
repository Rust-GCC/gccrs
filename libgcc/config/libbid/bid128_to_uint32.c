/* Copyright (C) 2007-2023 Free Software Foundation, Inc.

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

#include "bid_internal.h"

/*****************************************************************************
 *  BID128_to_uint32_rnint
 ****************************************************************************/

BID128_FUNCTION_ARG1_NORND_CUSTOMRESTYPE (unsigned int,
					  bid128_to_uint32_rnint, x)

     unsigned int res;
     UINT64 x_sign;
     UINT64 x_exp;
     int exp;			// unbiased exponent
  // Note: C1.w[1], C1.w[0] represent x_signif_hi, x_signif_lo (all are UINT64)
     UINT64 tmp64;
     BID_UI64DOUBLE tmp1;
     unsigned int x_nr_bits;
     int q, ind, shift;
     UINT128 C1, C;
     UINT128 Cstar;		// C* represents up to 34 decimal digits ~ 113 bits
     UINT256 fstar;
     UINT256 P256;

  // unpack x
x_sign = x.w[1] & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
x_exp = x.w[1] & MASK_EXP;	// biased and shifted left 49 bit positions
C1.w[1] = x.w[1] & MASK_COEFF;
C1.w[0] = x.w[0];

  // check for NaN or Infinity
if ((x.w[1] & MASK_SPECIAL) == MASK_SPECIAL) {
    // x is special
if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
  if ((x.w[1] & MASK_SNAN) == MASK_SNAN) {	// x is SNAN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is QNaN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
} else {	// x is not a NaN, so it must be infinity
  if (!x_sign) {	// x is +inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is -inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
}
}
  // check for non-canonical values (after the check for special values)
if ((C1.w[1] > 0x0001ed09bead87c0ull)
    || (C1.w[1] == 0x0001ed09bead87c0ull
	&& (C1.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull)) {
  res = 0x00000000;
  BID_RETURN (res);
} else if ((C1.w[1] == 0x0ull) && (C1.w[0] == 0x0ull)) {
  // x is 0
  res = 0x00000000;
  BID_RETURN (res);
} else {	// x is not special and is not zero

  // q = nr. of decimal digits in x
  //  determine first the nr. of bits in x
  if (C1.w[1] == 0) {
    if (C1.w[0] >= 0x0020000000000000ull) {	// x >= 2^53
      // split the 64-bit value in two 32-bit halves to avoid rounding errors
      if (C1.w[0] >= 0x0000000100000000ull) {	// x >= 2^32
	tmp1.d = (double) (C1.w[0] >> 32);	// exact conversion
	x_nr_bits =
	  33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      } else {	// x < 2^32
	tmp1.d = (double) (C1.w[0]);	// exact conversion
	x_nr_bits =
	  1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      }
    } else {	// if x < 2^53
      tmp1.d = (double) C1.w[0];	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// C1.w[1] != 0 => nr. bits = 64 + nr_bits (C1.w[1])
    tmp1.d = (double) C1.w[1];	// exact conversion
    x_nr_bits =
      65 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1.w[1] > nr_digits[x_nr_bits - 1].threshold_hi
	|| (C1.w[1] == nr_digits[x_nr_bits - 1].threshold_hi
	    && C1.w[0] >= nr_digits[x_nr_bits - 1].threshold_lo))
      q++;
  }
  exp = (x_exp >> 49) - 6176;
  if ((q + exp) > 10) {	// x >= 10^10 ~= 2^33.2... (cannot fit in 32 bits)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
    BID_RETURN (res);
  } else if ((q + exp) == 10) {	// x = c(0)c(1)...c(9).c(10)...c(q-1)
    // in this case 2^29.89... ~= 10^9 <= x < 10^10 ~= 2^33.2...
    // so x rounded to an integer may or may not fit in an unsigned 32-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 10'
    if (x_sign) {	// if n < 0 and q + exp = 10
      // if n < -1/2 then n cannot be converted to uint32 with RN
      // too large if c(0)c(1)...c(9).c(10)...c(q-1) > 1/2
      // <=> 0.c(0)c(1)...c(q-1) * 10^11 > 0x05, 1<=q<=34
      if (q <= 11) {
	tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
	// c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
	if (tmp64 > 0x05ull) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
	// 0.c(0)c(1)...c(q-1) * 10^11 > 0x05 <=>
	// C > 0x05 * 10^(q-11) where 1 <= q - 11 <= 23
	// (scale 1/2 up)
	tmp64 = 0x05ull;
	if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	  __mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
	} else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	  __mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
	}
	if (C1.w[1] > C.w[1] || (C1.w[1] == C.w[1] && C1.w[0] > C.w[0])) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      }
    } else {	// if n > 0 and q + exp = 10
      // if n >= 2^32 - 1/2 then n is too large
      // too large if c(0)c(1)...c(9).c(10)...c(q-1) >= 2^32-1/2
      // too large if 0.c(0)c(1)...c(q-1) * 10^11 >= 0x9fffffffb, 1<=q<=34
      if (q <= 11) {
	tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
	// c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
	if (tmp64 >= 0x9fffffffbull) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
	// 0.c(0)c(1)...c(q-1) * 10^11 >= 0x9fffffffb <=>
	// C >= 0x9fffffffb * 10^(q-11) where 1 <= q - 11 <= 23
	// (scale 2^32-1/2 up)
	tmp64 = 0x9fffffffbull;
	if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	  __mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
	} else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	  __mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
	}
	if (C1.w[1] > C.w[1]
	    || (C1.w[1] == C.w[1] && C1.w[0] >= C.w[0])) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      }
    }
  }
  // n is not too large to be converted to int32: -1/2 <= n < 2^32 - 1/2
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) < 0) {	// n = +/-0.0...c(0)c(1)...c(q-1)
    // return 0
    res = 0x00000000;
    BID_RETURN (res);
  } else if ((q + exp) == 0) {	// n = +/-0.c(0)c(1)...c(q-1)
    // if 0.c(0)c(1)...c(q-1) <= 0.5 <=> c(0)c(1)...c(q-1) <= 5 * 10^(q-1)
    //   res = 0
    // else if x > 0
    //   res = +1
    // else // if x < 0
    //   invalid exc
    ind = q - 1;
    if (ind <= 18) {	// 0 <= ind <= 18
      if ((C1.w[1] == 0) && (C1.w[0] <= midpoint64[ind])) {
	res = 0x00000000;	// return 0
      } else if (!x_sign) {	// n > 0
	res = 0x00000001;	// return +1
      } else {
	res = 0x80000000;
	*pfpsf |= INVALID_EXCEPTION;
      }
    } else {	// 19 <= ind <= 33
      if ((C1.w[1] < midpoint128[ind - 19].w[1])
	  || ((C1.w[1] == midpoint128[ind - 19].w[1])
	      && (C1.w[0] <= midpoint128[ind - 19].w[0]))) {
	res = 0x00000000;	// return 0
      } else if (!x_sign) {	// n > 0
	res = 0x00000001;	// return +1
      } else {
	res = 0x80000000;
	*pfpsf |= INVALID_EXCEPTION;
      }
    }
  } else {	// if (1 <= q + exp <= 10, 1 <= q <= 34, -33 <= exp <= 9)
    if (x_sign) {	// x <= -1
      // set invalid flag
      *pfpsf |= INVALID_EXCEPTION;
      // return Integer Indefinite
      res = 0x80000000;
      BID_RETURN (res);
    }
    // 1 <= x < 2^32-1/2 so x can be rounded
    // to nearest to a 32-bit unsigned integer
    if (exp < 0) {	// 2 <= q <= 34, -33 <= exp <= -1, 1 <= q + exp <= 10
      ind = -exp;	// 1 <= ind <= 33; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 = C1 + 1/2 * 10^ind where the result C1 fits in 127 bits
      tmp64 = C1.w[0];
      if (ind <= 19) {
	C1.w[0] = C1.w[0] + midpoint64[ind - 1];
      } else {
	C1.w[0] = C1.w[0] + midpoint128[ind - 20].w[0];
	C1.w[1] = C1.w[1] + midpoint128[ind - 20].w[1];
      }
      if (C1.w[0] < tmp64)
	C1.w[1]++;
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 33
      // kx = 10^(-x) = ten2mk128[ind - 1]
      // C* = (C1 + 1/2 * 10^x) * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 118 bits
      __mul_128x128_to_256 (P256, C1, ten2mk128[ind - 1]);
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[1] = P256.w[3];
	Cstar.w[0] = P256.w[2];
	fstar.w[3] = 0;
	fstar.w[2] = P256.w[2] & maskhigh128[ind - 1];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[1] = 0;
	Cstar.w[0] = P256.w[3];
	fstar.w[3] = P256.w[3] & maskhigh128[ind - 1];
	fstar.w[2] = P256.w[2];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      }
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind], e.g.
      // if x=1, T*=ten2mk128trunc[0]=0x19999999999999999999999999999999
      // if (0 < f* < 10^(-x)) then the result is a midpoint
      //   if floor(C*) is even then C* = floor(C*) - logical right
      //       shift; C* has p decimal digits, correct by Prop. 1)
      //   else if floor(C*) is odd C* = floor(C*)-1 (logical right
      //       shift; C* has p decimal digits, correct by Pr. 1)
      // else
      //   C* = floor(C*) (logical right shift; C has p decimal digits,
      //       correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-128 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 102
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[0] =
	  (Cstar.w[0] >> shift) | (Cstar.w[1] << (64 - shift));
	// redundant, it will be 0! Cstar.w[1] = (Cstar.w[1] >> shift);
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[0] = (Cstar.w[0] >> (shift - 64));	// 2 <= shift - 64 <= 38
      }
      // if the result was a midpoint it was rounded away from zero, so
      // it will need a correction
      // check for midpoints
      if ((fstar.w[3] == 0) && (fstar.w[2] == 0)
	  && (fstar.w[1] || fstar.w[0])
	  && (fstar.w[1] < ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] <= ten2mk128trunc[ind - 1].w[0]))) {
	// the result is a midpoint; round to nearest
	if (Cstar.w[0] & 0x01) {	// Cstar.w[0] is odd; MP in [EVEN, ODD]
	  // if floor(C*) is odd C = floor(C*) - 1; the result >= 1
	  Cstar.w[0]--;	// Cstar.w[0] is now even
	}	// else MP in [ODD, EVEN]
      }
      res = Cstar.w[0];	// the result is positive
    } else if (exp == 0) {
      // 1 <= q <= 10
      // res = C (exact)
      res = C1.w[0];
    } else {	// if (exp > 0) => 1 <= exp <= 9, 1 <= q < 9, 2 <= q + exp <= 10
      // res = C * 10^exp (exact)
      res = C1.w[0] * ten2k64[exp];
    }
  }
}

BID_RETURN (res);
}

/*****************************************************************************
 *  BID128_to_uint32_xrnint
 ****************************************************************************/

BID128_FUNCTION_ARG1_NORND_CUSTOMRESTYPE (unsigned int,
					  bid128_to_uint32_xrnint, x)

     unsigned int res;
     UINT64 x_sign;
     UINT64 x_exp;
     int exp;			// unbiased exponent
  // Note: C1.w[1], C1.w[0] represent x_signif_hi, x_signif_lo (all are UINT64)
     UINT64 tmp64, tmp64A;
     BID_UI64DOUBLE tmp1;
     unsigned int x_nr_bits;
     int q, ind, shift;
     UINT128 C1, C;
     UINT128 Cstar;		// C* represents up to 34 decimal digits ~ 113 bits
     UINT256 fstar;
     UINT256 P256;
     unsigned int tmp_inexact = 0;

  // unpack x
x_sign = x.w[1] & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
x_exp = x.w[1] & MASK_EXP;	// biased and shifted left 49 bit positions
C1.w[1] = x.w[1] & MASK_COEFF;
C1.w[0] = x.w[0];

  // check for NaN or Infinity
if ((x.w[1] & MASK_SPECIAL) == MASK_SPECIAL) {
    // x is special
if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
  if ((x.w[1] & MASK_SNAN) == MASK_SNAN) {	// x is SNAN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is QNaN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
} else {	// x is not a NaN, so it must be infinity
  if (!x_sign) {	// x is +inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is -inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
}
}
  // check for non-canonical values (after the check for special values)
if ((C1.w[1] > 0x0001ed09bead87c0ull)
    || (C1.w[1] == 0x0001ed09bead87c0ull
	&& (C1.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull)) {
  res = 0x00000000;
  BID_RETURN (res);
} else if ((C1.w[1] == 0x0ull) && (C1.w[0] == 0x0ull)) {
  // x is 0
  res = 0x00000000;
  BID_RETURN (res);
} else {	// x is not special and is not zero

  // q = nr. of decimal digits in x
  //  determine first the nr. of bits in x
  if (C1.w[1] == 0) {
    if (C1.w[0] >= 0x0020000000000000ull) {	// x >= 2^53
      // split the 64-bit value in two 32-bit halves to avoid rounding errors
      if (C1.w[0] >= 0x0000000100000000ull) {	// x >= 2^32
	tmp1.d = (double) (C1.w[0] >> 32);	// exact conversion
	x_nr_bits =
	  33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      } else {	// x < 2^32
	tmp1.d = (double) (C1.w[0]);	// exact conversion
	x_nr_bits =
	  1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      }
    } else {	// if x < 2^53
      tmp1.d = (double) C1.w[0];	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// C1.w[1] != 0 => nr. bits = 64 + nr_bits (C1.w[1])
    tmp1.d = (double) C1.w[1];	// exact conversion
    x_nr_bits =
      65 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1.w[1] > nr_digits[x_nr_bits - 1].threshold_hi
	|| (C1.w[1] == nr_digits[x_nr_bits - 1].threshold_hi
	    && C1.w[0] >= nr_digits[x_nr_bits - 1].threshold_lo))
      q++;
  }
  exp = (x_exp >> 49) - 6176;
  if ((q + exp) > 10) {	// x >= 10^10 ~= 2^33.2... (cannot fit in 32 bits)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
    BID_RETURN (res);
  } else if ((q + exp) == 10) {	// x = c(0)c(1)...c(9).c(10)...c(q-1)
    // in this case 2^29.89... ~= 10^9 <= x < 10^10 ~= 2^33.2...
    // so x rounded to an integer may or may not fit in an unsigned 32-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 10'
    if (x_sign) {	// if n < 0 and q + exp = 10
      // if n < -1/2 then n cannot be converted to uint32 with RN
      // too large if c(0)c(1)...c(9).c(10)...c(q-1) > 1/2
      // <=> 0.c(0)c(1)...c(q-1) * 10^11 > 0x05, 1<=q<=34
      if (q <= 11) {
	tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
	// c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
	if (tmp64 > 0x05ull) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
	// 0.c(0)c(1)...c(q-1) * 10^11 > 0x05 <=>
	// C > 0x05 * 10^(q-11) where 1 <= q - 11 <= 23
	// (scale 1/2 up)
	tmp64 = 0x05ull;
	if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	  __mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
	} else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	  __mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
	}
	if (C1.w[1] > C.w[1] || (C1.w[1] == C.w[1] && C1.w[0] > C.w[0])) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      }
    } else {	// if n > 0 and q + exp = 10
      // if n >= 2^32 - 1/2 then n is too large
      // too large if c(0)c(1)...c(9).c(10)...c(q-1) >= 2^32-1/2
      // too large if 0.c(0)c(1)...c(q-1) * 10^11 >= 0x9fffffffb, 1<=q<=34
      if (q <= 11) {
	tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
	// c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
	if (tmp64 >= 0x9fffffffbull) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
	// 0.c(0)c(1)...c(q-1) * 10^11 >= 0x9fffffffb <=>
	// C >= 0x9fffffffb * 10^(q-11) where 1 <= q - 11 <= 23
	// (scale 2^32-1/2 up)
	tmp64 = 0x9fffffffbull;
	if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	  __mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
	} else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	  __mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
	}
	if (C1.w[1] > C.w[1]
	    || (C1.w[1] == C.w[1] && C1.w[0] >= C.w[0])) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      }
    }
  }
  // n is not too large to be converted to int32: -1/2 <= n < 2^32 - 1/2
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) < 0) {	// n = +/-0.0...c(0)c(1)...c(q-1)
    // set inexact flag
    *pfpsf |= INEXACT_EXCEPTION;
    // return 0
    res = 0x00000000;
    BID_RETURN (res);
  } else if ((q + exp) == 0) {	// n = +/-0.c(0)c(1)...c(q-1)
    // if 0.c(0)c(1)...c(q-1) <= 0.5 <=> c(0)c(1)...c(q-1) <= 5 * 10^(q-1)
    //   res = 0
    // else if x > 0
    //   res = +1
    // else // if x < 0
    //   invalid exc
    ind = q - 1;
    if (ind <= 18) {	// 0 <= ind <= 18
      if ((C1.w[1] == 0) && (C1.w[0] <= midpoint64[ind])) {
	res = 0x00000000;	// return 0
      } else if (!x_sign) {	// n > 0
	res = 0x00000001;	// return +1
      } else {
	res = 0x80000000;
	*pfpsf |= INVALID_EXCEPTION;
	BID_RETURN (res);
      }
    } else {	// 19 <= ind <= 33
      if ((C1.w[1] < midpoint128[ind - 19].w[1])
	  || ((C1.w[1] == midpoint128[ind - 19].w[1])
	      && (C1.w[0] <= midpoint128[ind - 19].w[0]))) {
	res = 0x00000000;	// return 0
      } else if (!x_sign) {	// n > 0
	res = 0x00000001;	// return +1
      } else {
	res = 0x80000000;
	*pfpsf |= INVALID_EXCEPTION;
	BID_RETURN (res);
      }
    }
    // set inexact flag
    *pfpsf |= INEXACT_EXCEPTION;
  } else {	// if (1 <= q + exp <= 10, 1 <= q <= 34, -33 <= exp <= 9)
    if (x_sign) {	// x <= -1
      // set invalid flag
      *pfpsf |= INVALID_EXCEPTION;
      // return Integer Indefinite
      res = 0x80000000;
      BID_RETURN (res);
    }
    // 1 <= x < 2^32-1/2 so x can be rounded
    // to nearest to a 32-bit unsigned integer
    if (exp < 0) {	// 2 <= q <= 34, -33 <= exp <= -1, 1 <= q + exp <= 10
      ind = -exp;	// 1 <= ind <= 33; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 = C1 + 1/2 * 10^ind where the result C1 fits in 127 bits
      tmp64 = C1.w[0];
      if (ind <= 19) {
	C1.w[0] = C1.w[0] + midpoint64[ind - 1];
      } else {
	C1.w[0] = C1.w[0] + midpoint128[ind - 20].w[0];
	C1.w[1] = C1.w[1] + midpoint128[ind - 20].w[1];
      }
      if (C1.w[0] < tmp64)
	C1.w[1]++;
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 33
      // kx = 10^(-x) = ten2mk128[ind - 1]
      // C* = (C1 + 1/2 * 10^x) * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 118 bits
      __mul_128x128_to_256 (P256, C1, ten2mk128[ind - 1]);
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[1] = P256.w[3];
	Cstar.w[0] = P256.w[2];
	fstar.w[3] = 0;
	fstar.w[2] = P256.w[2] & maskhigh128[ind - 1];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[1] = 0;
	Cstar.w[0] = P256.w[3];
	fstar.w[3] = P256.w[3] & maskhigh128[ind - 1];
	fstar.w[2] = P256.w[2];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      }
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind], e.g.
      // if x=1, T*=ten2mk128trunc[0]=0x19999999999999999999999999999999
      // if (0 < f* < 10^(-x)) then the result is a midpoint
      //   if floor(C*) is even then C* = floor(C*) - logical right
      //       shift; C* has p decimal digits, correct by Prop. 1)
      //   else if floor(C*) is odd C* = floor(C*)-1 (logical right
      //       shift; C* has p decimal digits, correct by Pr. 1)
      // else
      //   C* = floor(C*) (logical right shift; C has p decimal digits,
      //       correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-128 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 102
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[0] =
	  (Cstar.w[0] >> shift) | (Cstar.w[1] << (64 - shift));
	// redundant, it will be 0! Cstar.w[1] = (Cstar.w[1] >> shift);
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[0] = (Cstar.w[0] >> (shift - 64));	// 2 <= shift - 64 <= 38
      }
      // determine inexactness of the rounding of C*
      // if (0 < f* - 1/2 < 10^(-x)) then
      //   the result is exact
      // else // if (f* - 1/2 > T*) then
      //   the result is inexact
      if (ind - 1 <= 2) {
	if (fstar.w[1] > 0x8000000000000000ull ||
	    (fstar.w[1] == 0x8000000000000000ull
	     && fstar.w[0] > 0x0ull)) {
	  // f* > 1/2 and the result may be exact
	  tmp64 = fstar.w[1] - 0x8000000000000000ull;	// f* - 1/2
	  if (tmp64 > ten2mk128trunc[ind - 1].w[1]
	      || (tmp64 == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] >= ten2mk128trunc[ind - 1].w[0])) {
	    // set the inexact flag
	    // *pfpsf |= INEXACT_EXCEPTION;
	    tmp_inexact = 1;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  // *pfpsf |= INEXACT_EXCEPTION;
	  tmp_inexact = 1;
	}
      } else if (ind - 1 <= 21) {	// if 3 <= ind <= 21
	if (fstar.w[3] > 0x0 ||
	    (fstar.w[3] == 0x0 && fstar.w[2] > onehalf128[ind - 1]) ||
	    (fstar.w[3] == 0x0 && fstar.w[2] == onehalf128[ind - 1] &&
	     (fstar.w[1] || fstar.w[0]))) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[2] - onehalf128[ind - 1];
	  tmp64A = fstar.w[3];
	  if (tmp64 > fstar.w[2])
	    tmp64A--;
	  if (tmp64A || tmp64
	      || fstar.w[1] > ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] > ten2mk128trunc[ind - 1].w[0])) {
	    // set the inexact flag
	    // *pfpsf |= INEXACT_EXCEPTION;
	    tmp_inexact = 1;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  // *pfpsf |= INEXACT_EXCEPTION;
	  tmp_inexact = 1;
	}
      } else {	// if 22 <= ind <= 33
	if (fstar.w[3] > onehalf128[ind - 1] ||
	    (fstar.w[3] == onehalf128[ind - 1] &&
	     (fstar.w[2] || fstar.w[1] || fstar.w[0]))) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[3] - onehalf128[ind - 1];
	  if (tmp64 || fstar.w[2]
	      || fstar.w[1] > ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] > ten2mk128trunc[ind - 1].w[0])) {
	    // set the inexact flag
	    // *pfpsf |= INEXACT_EXCEPTION;
	    tmp_inexact = 1;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  // *pfpsf |= INEXACT_EXCEPTION;
	  tmp_inexact = 1;
	}
      }

      // if the result was a midpoint it was rounded away from zero, so
      // it will need a correction
      // check for midpoints
      if ((fstar.w[3] == 0) && (fstar.w[2] == 0)
	  && (fstar.w[1] || fstar.w[0])
	  && (fstar.w[1] < ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] <= ten2mk128trunc[ind - 1].w[0]))) {
	// the result is a midpoint; round to nearest
	if (Cstar.w[0] & 0x01) {	// Cstar.w[0] is odd; MP in [EVEN, ODD]
	  // if floor(C*) is odd C = floor(C*) - 1; the result >= 1
	  Cstar.w[0]--;	// Cstar.w[0] is now even
	}	// else MP in [ODD, EVEN]
      }
      res = Cstar.w[0];	// the result is positive
      if (tmp_inexact)
	*pfpsf |= INEXACT_EXCEPTION;
    } else if (exp == 0) {
      // 1 <= q <= 10
      // res = C (exact)
      res = C1.w[0];
    } else {	// if (exp > 0) => 1 <= exp <= 9, 1 <= q < 9, 2 <= q + exp <= 10
      // res = C * 10^exp (exact)
      res = C1.w[0] * ten2k64[exp];
    }
  }
}

BID_RETURN (res);
}

/*****************************************************************************
 *  BID128_to_uint32_floor
 ****************************************************************************/

BID128_FUNCTION_ARG1_NORND_CUSTOMRESTYPE (unsigned int,
					  bid128_to_uint32_floor, x)

     unsigned int res;
     UINT64 x_sign;
     UINT64 x_exp;
     int exp;			// unbiased exponent
  // Note: C1.w[1], C1.w[0] represent x_signif_hi, x_signif_lo (all are UINT64)
     UINT64 tmp64, tmp64A;
     BID_UI64DOUBLE tmp1;
     unsigned int x_nr_bits;
     int q, ind, shift;
     UINT128 C1, C;
     UINT128 Cstar;		// C* represents up to 34 decimal digits ~ 113 bits
     UINT256 fstar;
     UINT256 P256;
     int is_inexact_gt_midpoint = 0;
     int is_midpoint_lt_even = 0;

  // unpack x
x_sign = x.w[1] & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
x_exp = x.w[1] & MASK_EXP;	// biased and shifted left 49 bit positions
C1.w[1] = x.w[1] & MASK_COEFF;
C1.w[0] = x.w[0];

  // check for NaN or Infinity
if ((x.w[1] & MASK_SPECIAL) == MASK_SPECIAL) {
    // x is special
if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
  if ((x.w[1] & MASK_SNAN) == MASK_SNAN) {	// x is SNAN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is QNaN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
} else {	// x is not a NaN, so it must be infinity
  if (!x_sign) {	// x is +inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is -inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
}
}
  // check for non-canonical values (after the check for special values)
if ((C1.w[1] > 0x0001ed09bead87c0ull)
    || (C1.w[1] == 0x0001ed09bead87c0ull
	&& (C1.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull)) {
  res = 0x00000000;
  BID_RETURN (res);
} else if ((C1.w[1] == 0x0ull) && (C1.w[0] == 0x0ull)) {
  // x is 0
  res = 0x00000000;
  BID_RETURN (res);
} else {	// x is not special and is not zero
  // x < 0 is invalid
  if (x_sign) {
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
    BID_RETURN (res);
  }
  // x > 0 from this point on
  // q = nr. of decimal digits in x
  //  determine first the nr. of bits in x
  if (C1.w[1] == 0) {
    if (C1.w[0] >= 0x0020000000000000ull) {	// x >= 2^53
      // split the 64-bit value in two 32-bit halves to avoid rounding errors
      if (C1.w[0] >= 0x0000000100000000ull) {	// x >= 2^32
	tmp1.d = (double) (C1.w[0] >> 32);	// exact conversion
	x_nr_bits =
	  33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      } else {	// x < 2^32
	tmp1.d = (double) (C1.w[0]);	// exact conversion
	x_nr_bits =
	  1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      }
    } else {	// if x < 2^53
      tmp1.d = (double) C1.w[0];	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// C1.w[1] != 0 => nr. bits = 64 + nr_bits (C1.w[1])
    tmp1.d = (double) C1.w[1];	// exact conversion
    x_nr_bits =
      65 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1.w[1] > nr_digits[x_nr_bits - 1].threshold_hi
	|| (C1.w[1] == nr_digits[x_nr_bits - 1].threshold_hi
	    && C1.w[0] >= nr_digits[x_nr_bits - 1].threshold_lo))
      q++;
  }
  exp = (x_exp >> 49) - 6176;

  if ((q + exp) > 10) {	// x >= 10^10 ~= 2^33.2... (cannot fit in 32 bits)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
    BID_RETURN (res);
  } else if ((q + exp) == 10) {	// x = c(0)c(1)...c(9).c(10)...c(q-1)
    // in this case 2^29.89... ~= 10^9 <= x < 10^10 ~= 2^33.2...
    // so x rounded to an integer may or may not fit in a signed 32-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 10'
    // n > 0 and q + exp = 10
    // if n >= 2^32 then n is too large
    // too large if c(0)c(1)...c(9).c(10)...c(q-1) >= 2^32
    // too large if 0.c(0)c(1)...c(q-1) * 10^11 >= 0xa00000000, 1<=q<=34
    if (q <= 11) {
      tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
      // c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
      if (tmp64 >= 0xa00000000ull) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x80000000;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 32-bit int fall through
      // to '1 <= q + exp <= 10'
    } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
      // 0.c(0)c(1)...c(q-1) * 10^11 >= 0xa00000000 <=>
      // C >= 0xa00000000 * 10^(q-11) where 1 <= q - 11 <= 23
      // (scale 2^32 up)
      tmp64 = 0xa00000000ull;
      if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	__mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
      } else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	__mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
      }
      if (C1.w[1] > C.w[1] || (C1.w[1] == C.w[1] && C1.w[0] >= C.w[0])) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x80000000;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 32-bit int fall through
      // to '1 <= q + exp <= 10'
    }
  }
  // n is not too large to be converted to int32: 0 <= n < 2^31
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) <= 0) {
    // n = +0.0...c(0)c(1)...c(q-1) or n = +0.c(0)c(1)...c(q-1)
    // return 0
    res = 0x00000000;
    BID_RETURN (res);
  } else {	// if (1 <= q + exp <= 10, 1 <= q <= 34, -33 <= exp <= 9)
    // 1 <= x < 2^32 so x can be rounded down to a 32-bit unsigned integer
    if (exp < 0) {	// 2 <= q <= 34, -33 <= exp <= -1, 1 <= q + exp <= 10
      ind = -exp;	// 1 <= ind <= 33; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 = C1 + 1/2 * 10^ind where the result C1 fits in 127 bits
      tmp64 = C1.w[0];
      if (ind <= 19) {
	C1.w[0] = C1.w[0] + midpoint64[ind - 1];
      } else {
	C1.w[0] = C1.w[0] + midpoint128[ind - 20].w[0];
	C1.w[1] = C1.w[1] + midpoint128[ind - 20].w[1];
      }
      if (C1.w[0] < tmp64)
	C1.w[1]++;
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 33
      // kx = 10^(-x) = ten2mk128[ind - 1]
      // C* = (C1 + 1/2 * 10^x) * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 118 bits
      __mul_128x128_to_256 (P256, C1, ten2mk128[ind - 1]);
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[1] = P256.w[3];
	Cstar.w[0] = P256.w[2];
	fstar.w[3] = 0;
	fstar.w[2] = P256.w[2] & maskhigh128[ind - 1];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[1] = 0;
	Cstar.w[0] = P256.w[3];
	fstar.w[3] = P256.w[3] & maskhigh128[ind - 1];
	fstar.w[2] = P256.w[2];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      }
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind], e.g.
      // if x=1, T*=ten2mk128trunc[0]=0x19999999999999999999999999999999
      // if (0 < f* < 10^(-x)) then the result is a midpoint
      //   if floor(C*) is even then C* = floor(C*) - logical right
      //       shift; C* has p decimal digits, correct by Prop. 1)
      //   else if floor(C*) is odd C* = floor(C*)-1 (logical right
      //       shift; C* has p decimal digits, correct by Pr. 1)
      // else
      //   C* = floor(C*) (logical right shift; C has p decimal digits,
      //       correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-128 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 102
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[0] =
	  (Cstar.w[0] >> shift) | (Cstar.w[1] << (64 - shift));
	// redundant, it will be 0! Cstar.w[1] = (Cstar.w[1] >> shift);
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[0] = (Cstar.w[0] >> (shift - 64));	// 2 <= shift - 64 <= 38
      }
      // determine inexactness of the rounding of C*
      // if (0 < f* - 1/2 < 10^(-x)) then
      //   the result is exact
      // else // if (f* - 1/2 > T*) then
      //   the result is inexact
      if (ind - 1 <= 2) {
	if (fstar.w[1] > 0x8000000000000000ull ||
	    (fstar.w[1] == 0x8000000000000000ull
	     && fstar.w[0] > 0x0ull)) {
	  // f* > 1/2 and the result may be exact
	  tmp64 = fstar.w[1] - 0x8000000000000000ull;	// f* - 1/2
	  if (tmp64 > ten2mk128trunc[ind - 1].w[1]
	      || (tmp64 == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] >= ten2mk128trunc[ind - 1].w[0])) {
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  is_inexact_gt_midpoint = 1;
	}
      } else if (ind - 1 <= 21) {	// if 3 <= ind <= 21
	if (fstar.w[3] > 0x0 ||
	    (fstar.w[3] == 0x0 && fstar.w[2] > onehalf128[ind - 1]) ||
	    (fstar.w[3] == 0x0 && fstar.w[2] == onehalf128[ind - 1] &&
	     (fstar.w[1] || fstar.w[0]))) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[2] - onehalf128[ind - 1];
	  tmp64A = fstar.w[3];
	  if (tmp64 > fstar.w[2])
	    tmp64A--;
	  if (tmp64A || tmp64
	      || fstar.w[1] > ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] > ten2mk128trunc[ind - 1].w[0])) {
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  is_inexact_gt_midpoint = 1;
	}
      } else {	// if 22 <= ind <= 33
	if (fstar.w[3] > onehalf128[ind - 1] ||
	    (fstar.w[3] == onehalf128[ind - 1] &&
	     (fstar.w[2] || fstar.w[1] || fstar.w[0]))) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[3] - onehalf128[ind - 1];
	  if (tmp64 || fstar.w[2]
	      || fstar.w[1] > ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] > ten2mk128trunc[ind - 1].w[0])) {
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  is_inexact_gt_midpoint = 1;
	}
      }

      // if the result was a midpoint it was rounded away from zero, so
      // it will need a correction
      // check for midpoints
      if ((fstar.w[3] == 0) && (fstar.w[2] == 0)
	  && (fstar.w[1] || fstar.w[0])
	  && (fstar.w[1] < ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] <= ten2mk128trunc[ind - 1].w[0]))) {
	// the result is a midpoint; round to nearest
	if (Cstar.w[0] & 0x01) {	// Cstar.w[0] is odd; MP in [EVEN, ODD]
	  // if floor(C*) is odd C = floor(C*) - 1; the result >= 1
	  Cstar.w[0]--;	// Cstar.w[0] is now even
	  is_inexact_gt_midpoint = 0;
	} else {	// else MP in [ODD, EVEN]
	  is_midpoint_lt_even = 1;
	  is_inexact_gt_midpoint = 0;
	}
      }
      // general correction for RM
      if (is_midpoint_lt_even || is_inexact_gt_midpoint) {
	Cstar.w[0] = Cstar.w[0] - 1;
      } else {
	;	// the result is already correct
      }
      res = Cstar.w[0];
    } else if (exp == 0) {
      // 1 <= q <= 10
      // res = +C (exact)
      res = C1.w[0];
    } else {	// if (exp > 0) => 1 <= exp <= 9, 1 <= q < 9, 2 <= q + exp <= 10
      // res = +C * 10^exp (exact)
      res = C1.w[0] * ten2k64[exp];
    }
  }
}

BID_RETURN (res);
}

/*****************************************************************************
 *  BID128_to_uint32_xfloor
 ****************************************************************************/

BID128_FUNCTION_ARG1_NORND_CUSTOMRESTYPE (unsigned int,
					  bid128_to_uint32_xfloor, x)

     unsigned int res;
     UINT64 x_sign;
     UINT64 x_exp;
     int exp;			// unbiased exponent
  // Note: C1.w[1], C1.w[0] represent x_signif_hi, x_signif_lo (all are UINT64)
     UINT64 tmp64, tmp64A;
     BID_UI64DOUBLE tmp1;
     unsigned int x_nr_bits;
     int q, ind, shift;
     UINT128 C1, C;
     UINT128 Cstar;		// C* represents up to 34 decimal digits ~ 113 bits
     UINT256 fstar;
     UINT256 P256;
     int is_inexact_gt_midpoint = 0;
     int is_midpoint_lt_even = 0;

  // unpack x
x_sign = x.w[1] & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
x_exp = x.w[1] & MASK_EXP;	// biased and shifted left 49 bit positions
C1.w[1] = x.w[1] & MASK_COEFF;
C1.w[0] = x.w[0];

  // check for NaN or Infinity
if ((x.w[1] & MASK_SPECIAL) == MASK_SPECIAL) {
    // x is special
if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
  if ((x.w[1] & MASK_SNAN) == MASK_SNAN) {	// x is SNAN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is QNaN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
} else {	// x is not a NaN, so it must be infinity
  if (!x_sign) {	// x is +inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is -inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
}
}
  // check for non-canonical values (after the check for special values)
if ((C1.w[1] > 0x0001ed09bead87c0ull)
    || (C1.w[1] == 0x0001ed09bead87c0ull
	&& (C1.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull)) {
  res = 0x00000000;
  BID_RETURN (res);
} else if ((C1.w[1] == 0x0ull) && (C1.w[0] == 0x0ull)) {
  // x is 0
  res = 0x00000000;
  BID_RETURN (res);
} else {	// x is not special and is not zero
  // x < 0 is invalid
  if (x_sign) {
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
    BID_RETURN (res);
  }
  // x > 0 from this point on
  // q = nr. of decimal digits in x
  //  determine first the nr. of bits in x
  if (C1.w[1] == 0) {
    if (C1.w[0] >= 0x0020000000000000ull) {	// x >= 2^53
      // split the 64-bit value in two 32-bit halves to avoid rounding errors
      if (C1.w[0] >= 0x0000000100000000ull) {	// x >= 2^32
	tmp1.d = (double) (C1.w[0] >> 32);	// exact conversion
	x_nr_bits =
	  33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      } else {	// x < 2^32
	tmp1.d = (double) (C1.w[0]);	// exact conversion
	x_nr_bits =
	  1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      }
    } else {	// if x < 2^53
      tmp1.d = (double) C1.w[0];	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// C1.w[1] != 0 => nr. bits = 64 + nr_bits (C1.w[1])
    tmp1.d = (double) C1.w[1];	// exact conversion
    x_nr_bits =
      65 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1.w[1] > nr_digits[x_nr_bits - 1].threshold_hi
	|| (C1.w[1] == nr_digits[x_nr_bits - 1].threshold_hi
	    && C1.w[0] >= nr_digits[x_nr_bits - 1].threshold_lo))
      q++;
  }
  exp = (x_exp >> 49) - 6176;
  if ((q + exp) > 10) {	// x >= 10^10 ~= 2^33.2... (cannot fit in 32 bits)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
    BID_RETURN (res);
  } else if ((q + exp) == 10) {	// x = c(0)c(1)...c(9).c(10)...c(q-1)
    // in this case 2^29.89... ~= 10^9 <= x < 10^10 ~= 2^33.2...
    // so x rounded to an integer may or may not fit in a signed 32-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 10'
    // n > 0 and q + exp = 10
    // if n >= 2^32 then n is too large
    // too large if c(0)c(1)...c(9).c(10)...c(q-1) >= 2^32
    // too large if 0.c(0)c(1)...c(q-1) * 10^11 >= 0xa00000000, 1<=q<=34
    if (q <= 11) {
      tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
      // c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
      if (tmp64 >= 0xa00000000ull) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x80000000;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 32-bit int fall through
      // to '1 <= q + exp <= 10'
    } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
      // 0.c(0)c(1)...c(q-1) * 10^11 >= 0xa00000000 <=>
      // C >= 0xa00000000 * 10^(q-11) where 1 <= q - 11 <= 23
      // (scale 2^32 up)
      tmp64 = 0xa00000000ull;
      if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	__mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
      } else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	__mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
      }
      if (C1.w[1] > C.w[1] || (C1.w[1] == C.w[1] && C1.w[0] >= C.w[0])) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x80000000;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 32-bit int fall through
      // to '1 <= q + exp <= 10'
    }
  }
  // n is not too large to be converted to int32: 0 <= n < 2^31
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) <= 0) {
    // n = +0.0...c(0)c(1)...c(q-1) or n = +0.c(0)c(1)...c(q-1)
    // set inexact flag
    *pfpsf |= INEXACT_EXCEPTION;
    // return 0
    res = 0x00000000;
    BID_RETURN (res);
  } else {	// if (1 <= q + exp <= 10, 1 <= q <= 34, -33 <= exp <= 9)
    // 1 <= x < 2^32 so x can be rounded down to a 32-bit unsigned integer
    if (exp < 0) {	// 2 <= q <= 34, -33 <= exp <= -1, 1 <= q + exp <= 10
      ind = -exp;	// 1 <= ind <= 33; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 = C1 + 1/2 * 10^ind where the result C1 fits in 127 bits
      tmp64 = C1.w[0];
      if (ind <= 19) {
	C1.w[0] = C1.w[0] + midpoint64[ind - 1];
      } else {
	C1.w[0] = C1.w[0] + midpoint128[ind - 20].w[0];
	C1.w[1] = C1.w[1] + midpoint128[ind - 20].w[1];
      }
      if (C1.w[0] < tmp64)
	C1.w[1]++;
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 33
      // kx = 10^(-x) = ten2mk128[ind - 1]
      // C* = (C1 + 1/2 * 10^x) * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 118 bits
      __mul_128x128_to_256 (P256, C1, ten2mk128[ind - 1]);
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[1] = P256.w[3];
	Cstar.w[0] = P256.w[2];
	fstar.w[3] = 0;
	fstar.w[2] = P256.w[2] & maskhigh128[ind - 1];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[1] = 0;
	Cstar.w[0] = P256.w[3];
	fstar.w[3] = P256.w[3] & maskhigh128[ind - 1];
	fstar.w[2] = P256.w[2];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      }
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind], e.g.
      // if x=1, T*=ten2mk128trunc[0]=0x19999999999999999999999999999999
      // if (0 < f* < 10^(-x)) then the result is a midpoint
      //   if floor(C*) is even then C* = floor(C*) - logical right
      //       shift; C* has p decimal digits, correct by Prop. 1)
      //   else if floor(C*) is odd C* = floor(C*)-1 (logical right
      //       shift; C* has p decimal digits, correct by Pr. 1)
      // else
      //   C* = floor(C*) (logical right shift; C has p decimal digits,
      //       correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-128 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 102
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[0] =
	  (Cstar.w[0] >> shift) | (Cstar.w[1] << (64 - shift));
	// redundant, it will be 0! Cstar.w[1] = (Cstar.w[1] >> shift);
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[0] = (Cstar.w[0] >> (shift - 64));	// 2 <= shift - 64 <= 38
      }
      // determine inexactness of the rounding of C*
      // if (0 < f* - 1/2 < 10^(-x)) then
      //   the result is exact
      // else // if (f* - 1/2 > T*) then
      //   the result is inexact
      if (ind - 1 <= 2) {
	if (fstar.w[1] > 0x8000000000000000ull ||
	    (fstar.w[1] == 0x8000000000000000ull
	     && fstar.w[0] > 0x0ull)) {
	  // f* > 1/2 and the result may be exact
	  tmp64 = fstar.w[1] - 0x8000000000000000ull;	// f* - 1/2
	  if (tmp64 > ten2mk128trunc[ind - 1].w[1]
	      || (tmp64 == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] >= ten2mk128trunc[ind - 1].w[0])) {
	    // set the inexact flag
	    *pfpsf |= INEXACT_EXCEPTION;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	  is_inexact_gt_midpoint = 1;
	}
      } else if (ind - 1 <= 21) {	// if 3 <= ind <= 21
	if (fstar.w[3] > 0x0 ||
	    (fstar.w[3] == 0x0 && fstar.w[2] > onehalf128[ind - 1]) ||
	    (fstar.w[3] == 0x0 && fstar.w[2] == onehalf128[ind - 1] &&
	     (fstar.w[1] || fstar.w[0]))) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[2] - onehalf128[ind - 1];
	  tmp64A = fstar.w[3];
	  if (tmp64 > fstar.w[2])
	    tmp64A--;
	  if (tmp64A || tmp64
	      || fstar.w[1] > ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] > ten2mk128trunc[ind - 1].w[0])) {
	    // set the inexact flag
	    *pfpsf |= INEXACT_EXCEPTION;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	  is_inexact_gt_midpoint = 1;
	}
      } else {	// if 22 <= ind <= 33
	if (fstar.w[3] > onehalf128[ind - 1] ||
	    (fstar.w[3] == onehalf128[ind - 1] &&
	     (fstar.w[2] || fstar.w[1] || fstar.w[0]))) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[3] - onehalf128[ind - 1];
	  if (tmp64 || fstar.w[2]
	      || fstar.w[1] > ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] > ten2mk128trunc[ind - 1].w[0])) {
	    // set the inexact flag
	    *pfpsf |= INEXACT_EXCEPTION;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	  is_inexact_gt_midpoint = 1;
	}
      }

      // if the result was a midpoint it was rounded away from zero, so
      // it will need a correction
      // check for midpoints
      if ((fstar.w[3] == 0) && (fstar.w[2] == 0)
	  && (fstar.w[1] || fstar.w[0])
	  && (fstar.w[1] < ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] <= ten2mk128trunc[ind - 1].w[0]))) {
	// the result is a midpoint; round to nearest
	if (Cstar.w[0] & 0x01) {	// Cstar.w[0] is odd; MP in [EVEN, ODD]
	  // if floor(C*) is odd C = floor(C*) - 1; the result >= 1
	  Cstar.w[0]--;	// Cstar.w[0] is now even
	  is_inexact_gt_midpoint = 0;
	} else {	// else MP in [ODD, EVEN]
	  is_midpoint_lt_even = 1;
	  is_inexact_gt_midpoint = 0;
	}
      }
      // general correction for RM
      if (is_midpoint_lt_even || is_inexact_gt_midpoint) {
	Cstar.w[0] = Cstar.w[0] - 1;
      } else {
	;	// the result is already correct
      }
      res = Cstar.w[0];
    } else if (exp == 0) {
      // 1 <= q <= 10
      // res = +C (exact)
      res = C1.w[0];
    } else {	// if (exp > 0) => 1 <= exp <= 9, 1 <= q < 9, 2 <= q + exp <= 10
      // res = +C * 10^exp (exact)
      res = C1.w[0] * ten2k64[exp];
    }
  }
}

BID_RETURN (res);
}

/*****************************************************************************
 *  BID128_to_uint32_ceil
 ****************************************************************************/

BID128_FUNCTION_ARG1_NORND_CUSTOMRESTYPE (unsigned int,
					  bid128_to_uint32_ceil, x)

     unsigned int res;
     UINT64 x_sign;
     UINT64 x_exp;
     int exp;			// unbiased exponent
  // Note: C1.w[1], C1.w[0] represent x_signif_hi, x_signif_lo (all are UINT64)
     UINT64 tmp64, tmp64A;
     BID_UI64DOUBLE tmp1;
     unsigned int x_nr_bits;
     int q, ind, shift;
     UINT128 C1, C;
     UINT128 Cstar;		// C* represents up to 34 decimal digits ~ 113 bits
     UINT256 fstar;
     UINT256 P256;
     int is_inexact_lt_midpoint = 0;
     int is_midpoint_gt_even = 0;

  // unpack x
x_sign = x.w[1] & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
x_exp = x.w[1] & MASK_EXP;	// biased and shifted left 49 bit positions
C1.w[1] = x.w[1] & MASK_COEFF;
C1.w[0] = x.w[0];

  // check for NaN or Infinity
if ((x.w[1] & MASK_SPECIAL) == MASK_SPECIAL) {
    // x is special
if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
  if ((x.w[1] & MASK_SNAN) == MASK_SNAN) {	// x is SNAN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is QNaN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
} else {	// x is not a NaN, so it must be infinity
  if (!x_sign) {	// x is +inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is -inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
}
}
  // check for non-canonical values (after the check for special values)
if ((C1.w[1] > 0x0001ed09bead87c0ull)
    || (C1.w[1] == 0x0001ed09bead87c0ull
	&& (C1.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull)) {
  res = 0x00000000;
  BID_RETURN (res);
} else if ((C1.w[1] == 0x0ull) && (C1.w[0] == 0x0ull)) {
  // x is 0
  res = 0x00000000;
  BID_RETURN (res);
} else {	// x is not special and is not zero

  // q = nr. of decimal digits in x
  //  determine first the nr. of bits in x
  if (C1.w[1] == 0) {
    if (C1.w[0] >= 0x0020000000000000ull) {	// x >= 2^53
      // split the 64-bit value in two 32-bit halves to avoid rounding errors
      if (C1.w[0] >= 0x0000000100000000ull) {	// x >= 2^32
	tmp1.d = (double) (C1.w[0] >> 32);	// exact conversion
	x_nr_bits =
	  33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      } else {	// x < 2^32
	tmp1.d = (double) (C1.w[0]);	// exact conversion
	x_nr_bits =
	  1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      }
    } else {	// if x < 2^53
      tmp1.d = (double) C1.w[0];	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// C1.w[1] != 0 => nr. bits = 64 + nr_bits (C1.w[1])
    tmp1.d = (double) C1.w[1];	// exact conversion
    x_nr_bits =
      65 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1.w[1] > nr_digits[x_nr_bits - 1].threshold_hi
	|| (C1.w[1] == nr_digits[x_nr_bits - 1].threshold_hi
	    && C1.w[0] >= nr_digits[x_nr_bits - 1].threshold_lo))
      q++;
  }
  exp = (x_exp >> 49) - 6176;

  if ((q + exp) > 10) {	// x >= 10^10 ~= 2^33.2... (cannot fit in 32 bits)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
    BID_RETURN (res);
  } else if ((q + exp) == 10) {	// x = c(0)c(1)...c(9).c(10)...c(q-1)
    // in this case 2^29.89... ~= 10^9 <= x < 10^10 ~= 2^33.2...
    // so x rounded to an integer may or may not fit in a signed 32-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 10'
    if (x_sign) {	// if n < 0 and q + exp = 10
      // if n <= -1 then n is too large
      // too large if c(0)c(1)...c(9).c(10)...c(q-1) >= 1
      // <=> 0.c(0)c(1)...c(q-1) * 10^11 >= 0x50000000a, 1<=q<=34
      if (q <= 11) {
	tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
	// c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
	if (tmp64 >= 0x0aull) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
	// 0.c(0)c(1)...c(q-1) * 10^11 >= 0x0a <=>
	// C >= 0x0a * 10^(q-11) where 1 <= q - 11 <= 23
	// (scale 1 up)
	tmp64 = 0x0aull;
	if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	  __mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
	} else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	  __mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
	}
	if (C1.w[1] > C.w[1]
	    || (C1.w[1] == C.w[1] && C1.w[0] >= C.w[0])) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      }
    } else {	// if n > 0 and q + exp = 10
      // if n > 2^32 - 1 then n is too large
      // too large if c(0)c(1)...c(9).c(10)...c(q-1) > 2^32 - 1
      // too large if 0.c(0)c(1)...c(q-1) * 10^11 > 0x9fffffff6, 1<=q<=34
      if (q <= 11) {
	tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
	// c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
	if (tmp64 > 0x9fffffff6ull) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
	// 0.c(0)c(1)...c(q-1) * 10^11 > 0x9fffffff6 <=>
	// C > 0x9fffffff6 * 10^(q-11) where 1 <= q - 11 <= 23
	// (scale 2^32 up)
	tmp64 = 0x9fffffff6ull;
	if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	  __mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
	} else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	  __mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
	}
	if (C1.w[1] > C.w[1] || (C1.w[1] == C.w[1] && C1.w[0] > C.w[0])) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      }
    }
  }
  // n is not too large to be converted to int32: -2^32-1 < n <= 2^32-1
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) <= 0) {
    // n = +/-0.0...c(0)c(1)...c(q-1) or n = +/-0.c(0)c(1)...c(q-1)
    // return 0
    if (x_sign)
      res = 0x00000000;
    else
      res = 0x00000001;
    BID_RETURN (res);
  } else {	// if (1 <= q + exp <= 10, 1 <= q <= 34, -33 <= exp <= 9)
    // -2^32-1 < x <= -1 or 1 <= x <= 2^32-1 so x can be rounded
    // toward positive infinity to a 32-bit signed integer
    if (x_sign) {	// x <= -1 is invalid
      // set invalid flag
      *pfpsf |= INVALID_EXCEPTION;
      // return Integer Indefinite
      res = 0x80000000;
      BID_RETURN (res);
    }
    // x > 0 from this point on
    if (exp < 0) {	// 2 <= q <= 34, -33 <= exp <= -1, 1 <= q + exp <= 10
      ind = -exp;	// 1 <= ind <= 33; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 = C1 + 1/2 * 10^ind where the result C1 fits in 127 bits
      tmp64 = C1.w[0];
      if (ind <= 19) {
	C1.w[0] = C1.w[0] + midpoint64[ind - 1];
      } else {
	C1.w[0] = C1.w[0] + midpoint128[ind - 20].w[0];
	C1.w[1] = C1.w[1] + midpoint128[ind - 20].w[1];
      }
      if (C1.w[0] < tmp64)
	C1.w[1]++;
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 33
      // kx = 10^(-x) = ten2mk128[ind - 1]
      // C* = (C1 + 1/2 * 10^x) * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 118 bits
      __mul_128x128_to_256 (P256, C1, ten2mk128[ind - 1]);
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[1] = P256.w[3];
	Cstar.w[0] = P256.w[2];
	fstar.w[3] = 0;
	fstar.w[2] = P256.w[2] & maskhigh128[ind - 1];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[1] = 0;
	Cstar.w[0] = P256.w[3];
	fstar.w[3] = P256.w[3] & maskhigh128[ind - 1];
	fstar.w[2] = P256.w[2];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      }
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind], e.g.
      // if x=1, T*=ten2mk128trunc[0]=0x19999999999999999999999999999999
      // if (0 < f* < 10^(-x)) then the result is a midpoint
      //   if floor(C*) is even then C* = floor(C*) - logical right
      //       shift; C* has p decimal digits, correct by Prop. 1)
      //   else if floor(C*) is odd C* = floor(C*)-1 (logical right
      //       shift; C* has p decimal digits, correct by Pr. 1)
      // else
      //   C* = floor(C*) (logical right shift; C has p decimal digits,
      //       correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-128 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 102
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[0] =
	  (Cstar.w[0] >> shift) | (Cstar.w[1] << (64 - shift));
	// redundant, it will be 0! Cstar.w[1] = (Cstar.w[1] >> shift);
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[0] = (Cstar.w[0] >> (shift - 64));	// 2 <= shift - 64 <= 38
      }
      // determine inexactness of the rounding of C*
      // if (0 < f* - 1/2 < 10^(-x)) then
      //   the result is exact
      // else // if (f* - 1/2 > T*) then
      //   the result is inexact
      if (ind - 1 <= 2) {
	if (fstar.w[1] > 0x8000000000000000ull ||
	    (fstar.w[1] == 0x8000000000000000ull
	     && fstar.w[0] > 0x0ull)) {
	  // f* > 1/2 and the result may be exact
	  tmp64 = fstar.w[1] - 0x8000000000000000ull;	// f* - 1/2
	  if (tmp64 > ten2mk128trunc[ind - 1].w[1]
	      || (tmp64 == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] >= ten2mk128trunc[ind - 1].w[0])) {
	    is_inexact_lt_midpoint = 1;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  ;
	}
      } else if (ind - 1 <= 21) {	// if 3 <= ind <= 21
	if (fstar.w[3] > 0x0 ||
	    (fstar.w[3] == 0x0 && fstar.w[2] > onehalf128[ind - 1]) ||
	    (fstar.w[3] == 0x0 && fstar.w[2] == onehalf128[ind - 1] &&
	     (fstar.w[1] || fstar.w[0]))) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[2] - onehalf128[ind - 1];
	  tmp64A = fstar.w[3];
	  if (tmp64 > fstar.w[2])
	    tmp64A--;
	  if (tmp64A || tmp64
	      || fstar.w[1] > ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] > ten2mk128trunc[ind - 1].w[0])) {
	    is_inexact_lt_midpoint = 1;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	}
      } else {	// if 22 <= ind <= 33
	if (fstar.w[3] > onehalf128[ind - 1] ||
	    (fstar.w[3] == onehalf128[ind - 1] &&
	     (fstar.w[2] || fstar.w[1] || fstar.w[0]))) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[3] - onehalf128[ind - 1];
	  if (tmp64 || fstar.w[2]
	      || fstar.w[1] > ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] > ten2mk128trunc[ind - 1].w[0])) {
	    is_inexact_lt_midpoint = 1;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  ;
	}
      }

      // if the result was a midpoint it was rounded away from zero, so
      // it will need a correction
      // check for midpoints
      if ((fstar.w[3] == 0) && (fstar.w[2] == 0)
	  && (fstar.w[1] || fstar.w[0])
	  && (fstar.w[1] < ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] <= ten2mk128trunc[ind - 1].w[0]))) {
	// the result is a midpoint; round to nearest
	if (Cstar.w[0] & 0x01) {	// Cstar.w[0] is odd; MP in [EVEN, ODD]
	  // if floor(C*) is odd C = floor(C*) - 1; the result >= 1
	  Cstar.w[0]--;	// Cstar.w[0] is now even
	  is_midpoint_gt_even = 1;
	  is_inexact_lt_midpoint = 0;
	} else {	// else MP in [ODD, EVEN]
	  is_inexact_lt_midpoint = 0;
	}
      }
      // general correction for RM
      if (is_midpoint_gt_even || is_inexact_lt_midpoint) {
	Cstar.w[0] = Cstar.w[0] + 1;
      } else {
	;	// the result is already correct
      }
      res = Cstar.w[0];
    } else if (exp == 0) {
      // 1 <= q <= 10
      // res = +C (exact)
      res = C1.w[0];
    } else {	// if (exp > 0) => 1 <= exp <= 9, 1 <= q < 9, 2 <= q + exp <= 10
      // res = +C * 10^exp (exact)
      res = C1.w[0] * ten2k64[exp];
    }
  }
}

BID_RETURN (res);
}

/*****************************************************************************
 *  BID128_to_uint32_xceil
 ****************************************************************************/

BID128_FUNCTION_ARG1_NORND_CUSTOMRESTYPE (unsigned int,
					  bid128_to_uint32_xceil, x)

     unsigned int res;
     UINT64 x_sign;
     UINT64 x_exp;
     int exp;			// unbiased exponent
  // Note: C1.w[1], C1.w[0] represent x_signif_hi, x_signif_lo (all are UINT64)
     UINT64 tmp64, tmp64A;
     BID_UI64DOUBLE tmp1;
     unsigned int x_nr_bits;
     int q, ind, shift;
     UINT128 C1, C;
     UINT128 Cstar;		// C* represents up to 34 decimal digits ~ 113 bits
     UINT256 fstar;
     UINT256 P256;
     int is_inexact_lt_midpoint = 0;
     int is_midpoint_gt_even = 0;

  // unpack x
x_sign = x.w[1] & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
x_exp = x.w[1] & MASK_EXP;	// biased and shifted left 49 bit positions
C1.w[1] = x.w[1] & MASK_COEFF;
C1.w[0] = x.w[0];

  // check for NaN or Infinity
if ((x.w[1] & MASK_SPECIAL) == MASK_SPECIAL) {
    // x is special
if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
  if ((x.w[1] & MASK_SNAN) == MASK_SNAN) {	// x is SNAN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is QNaN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
} else {	// x is not a NaN, so it must be infinity
  if (!x_sign) {	// x is +inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is -inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
}
}
  // check for non-canonical values (after the check for special values)
if ((C1.w[1] > 0x0001ed09bead87c0ull)
    || (C1.w[1] == 0x0001ed09bead87c0ull
	&& (C1.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull)) {
  res = 0x00000000;
  BID_RETURN (res);
} else if ((C1.w[1] == 0x0ull) && (C1.w[0] == 0x0ull)) {
  // x is 0
  res = 0x00000000;
  BID_RETURN (res);
} else {	// x is not special and is not zero

  // q = nr. of decimal digits in x
  //  determine first the nr. of bits in x
  if (C1.w[1] == 0) {
    if (C1.w[0] >= 0x0020000000000000ull) {	// x >= 2^53
      // split the 64-bit value in two 32-bit halves to avoid rounding errors
      if (C1.w[0] >= 0x0000000100000000ull) {	// x >= 2^32
	tmp1.d = (double) (C1.w[0] >> 32);	// exact conversion
	x_nr_bits =
	  33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      } else {	// x < 2^32
	tmp1.d = (double) (C1.w[0]);	// exact conversion
	x_nr_bits =
	  1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      }
    } else {	// if x < 2^53
      tmp1.d = (double) C1.w[0];	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// C1.w[1] != 0 => nr. bits = 64 + nr_bits (C1.w[1])
    tmp1.d = (double) C1.w[1];	// exact conversion
    x_nr_bits =
      65 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1.w[1] > nr_digits[x_nr_bits - 1].threshold_hi
	|| (C1.w[1] == nr_digits[x_nr_bits - 1].threshold_hi
	    && C1.w[0] >= nr_digits[x_nr_bits - 1].threshold_lo))
      q++;
  }
  exp = (x_exp >> 49) - 6176;
  if ((q + exp) > 10) {	// x >= 10^10 ~= 2^33.2... (cannot fit in 32 bits)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
    BID_RETURN (res);
  } else if ((q + exp) == 10) {	// x = c(0)c(1)...c(9).c(10)...c(q-1)
    // in this case 2^29.89... ~= 10^9 <= x < 10^10 ~= 2^33.2...
    // so x rounded to an integer may or may not fit in a signed 32-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 10'
    if (x_sign) {	// if n < 0 and q + exp = 10
      // if n <= -1 then n is too large
      // too large if c(0)c(1)...c(9).c(10)...c(q-1) >= 1
      // <=> 0.c(0)c(1)...c(q-1) * 10^11 >= 0x50000000a, 1<=q<=34
      if (q <= 11) {
	tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
	// c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
	if (tmp64 >= 0x0aull) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
	// 0.c(0)c(1)...c(q-1) * 10^11 >= 0x0a <=>
	// C >= 0x0a * 10^(q-11) where 1 <= q - 11 <= 23
	// (scale 1 up)
	tmp64 = 0x0aull;
	if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	  __mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
	} else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	  __mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
	}
	if (C1.w[1] > C.w[1]
	    || (C1.w[1] == C.w[1] && C1.w[0] >= C.w[0])) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      }
    } else {	// if n > 0 and q + exp = 10
      // if n > 2^32 - 1 then n is too large
      // too large if c(0)c(1)...c(9).c(10)...c(q-1) > 2^32 - 1
      // too large if 0.c(0)c(1)...c(q-1) * 10^11 > 0x9fffffff6, 1<=q<=34
      if (q <= 11) {
	tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
	// c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
	if (tmp64 > 0x9fffffff6ull) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
	// 0.c(0)c(1)...c(q-1) * 10^11 > 0x9fffffff6 <=>
	// C > 0x9fffffff6 * 10^(q-11) where 1 <= q - 11 <= 23
	// (scale 2^32 up)
	tmp64 = 0x9fffffff6ull;
	if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	  __mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
	} else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	  __mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
	}
	if (C1.w[1] > C.w[1] || (C1.w[1] == C.w[1] && C1.w[0] > C.w[0])) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      }
    }
  }
  // n is not too large to be converted to int32: -2^32-1 < n <= 2^32-1
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) <= 0) {
    // n = +/-0.0...c(0)c(1)...c(q-1) or n = +/-0.c(0)c(1)...c(q-1)
    // set inexact flag
    *pfpsf |= INEXACT_EXCEPTION;
    // return 0
    if (x_sign)
      res = 0x00000000;
    else
      res = 0x00000001;
    BID_RETURN (res);
  } else {	// if (1 <= q + exp <= 10, 1 <= q <= 34, -33 <= exp <= 9)
    // -2^32-1 < x <= -1 or 1 <= x <= 2^32-1 so x can be rounded
    // toward positive infinity to a 32-bit signed integer
    if (x_sign) {	// x <= -1 is invalid
      // set invalid flag
      *pfpsf |= INVALID_EXCEPTION;
      // return Integer Indefinite
      res = 0x80000000;
      BID_RETURN (res);
    }
    // x > 0 from this point on
    if (exp < 0) {	// 2 <= q <= 34, -33 <= exp <= -1, 1 <= q + exp <= 10
      ind = -exp;	// 1 <= ind <= 33; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 = C1 + 1/2 * 10^ind where the result C1 fits in 127 bits
      tmp64 = C1.w[0];
      if (ind <= 19) {
	C1.w[0] = C1.w[0] + midpoint64[ind - 1];
      } else {
	C1.w[0] = C1.w[0] + midpoint128[ind - 20].w[0];
	C1.w[1] = C1.w[1] + midpoint128[ind - 20].w[1];
      }
      if (C1.w[0] < tmp64)
	C1.w[1]++;
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 33
      // kx = 10^(-x) = ten2mk128[ind - 1]
      // C* = (C1 + 1/2 * 10^x) * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 118 bits
      __mul_128x128_to_256 (P256, C1, ten2mk128[ind - 1]);
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[1] = P256.w[3];
	Cstar.w[0] = P256.w[2];
	fstar.w[3] = 0;
	fstar.w[2] = P256.w[2] & maskhigh128[ind - 1];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[1] = 0;
	Cstar.w[0] = P256.w[3];
	fstar.w[3] = P256.w[3] & maskhigh128[ind - 1];
	fstar.w[2] = P256.w[2];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      }
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind], e.g.
      // if x=1, T*=ten2mk128trunc[0]=0x19999999999999999999999999999999
      // if (0 < f* < 10^(-x)) then the result is a midpoint
      //   if floor(C*) is even then C* = floor(C*) - logical right
      //       shift; C* has p decimal digits, correct by Prop. 1)
      //   else if floor(C*) is odd C* = floor(C*)-1 (logical right
      //       shift; C* has p decimal digits, correct by Pr. 1)
      // else
      //   C* = floor(C*) (logical right shift; C has p decimal digits,
      //       correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-128 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 102
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[0] =
	  (Cstar.w[0] >> shift) | (Cstar.w[1] << (64 - shift));
	// redundant, it will be 0! Cstar.w[1] = (Cstar.w[1] >> shift);
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[0] = (Cstar.w[0] >> (shift - 64));	// 2 <= shift - 64 <= 38
      }
      // determine inexactness of the rounding of C*
      // if (0 < f* - 1/2 < 10^(-x)) then
      //   the result is exact
      // else // if (f* - 1/2 > T*) then
      //   the result is inexact
      if (ind - 1 <= 2) {
	if (fstar.w[1] > 0x8000000000000000ull ||
	    (fstar.w[1] == 0x8000000000000000ull
	     && fstar.w[0] > 0x0ull)) {
	  // f* > 1/2 and the result may be exact
	  tmp64 = fstar.w[1] - 0x8000000000000000ull;	// f* - 1/2
	  if (tmp64 > ten2mk128trunc[ind - 1].w[1]
	      || (tmp64 == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] >= ten2mk128trunc[ind - 1].w[0])) {
	    // set the inexact flag
	    *pfpsf |= INEXACT_EXCEPTION;
	    is_inexact_lt_midpoint = 1;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	}
      } else if (ind - 1 <= 21) {	// if 3 <= ind <= 21
	if (fstar.w[3] > 0x0 ||
	    (fstar.w[3] == 0x0 && fstar.w[2] > onehalf128[ind - 1]) ||
	    (fstar.w[3] == 0x0 && fstar.w[2] == onehalf128[ind - 1] &&
	     (fstar.w[1] || fstar.w[0]))) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[2] - onehalf128[ind - 1];
	  tmp64A = fstar.w[3];
	  if (tmp64 > fstar.w[2])
	    tmp64A--;
	  if (tmp64A || tmp64
	      || fstar.w[1] > ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] > ten2mk128trunc[ind - 1].w[0])) {
	    // set the inexact flag
	    *pfpsf |= INEXACT_EXCEPTION;
	    is_inexact_lt_midpoint = 1;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	}
      } else {	// if 22 <= ind <= 33
	if (fstar.w[3] > onehalf128[ind - 1] ||
	    (fstar.w[3] == onehalf128[ind - 1] &&
	     (fstar.w[2] || fstar.w[1] || fstar.w[0]))) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[3] - onehalf128[ind - 1];
	  if (tmp64 || fstar.w[2]
	      || fstar.w[1] > ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] > ten2mk128trunc[ind - 1].w[0])) {
	    // set the inexact flag
	    *pfpsf |= INEXACT_EXCEPTION;
	    is_inexact_lt_midpoint = 1;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	}
      }

      // if the result was a midpoint it was rounded away from zero, so
      // it will need a correction
      // check for midpoints
      if ((fstar.w[3] == 0) && (fstar.w[2] == 0)
	  && (fstar.w[1] || fstar.w[0])
	  && (fstar.w[1] < ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] <= ten2mk128trunc[ind - 1].w[0]))) {
	// the result is a midpoint; round to nearest
	if (Cstar.w[0] & 0x01) {	// Cstar.w[0] is odd; MP in [EVEN, ODD]
	  // if floor(C*) is odd C = floor(C*) - 1; the result >= 1
	  Cstar.w[0]--;	// Cstar.w[0] is now even
	  is_midpoint_gt_even = 1;
	  is_inexact_lt_midpoint = 0;
	} else {	// else MP in [ODD, EVEN]
	  is_inexact_lt_midpoint = 0;
	}
      }
      // general correction for RM
      if (is_midpoint_gt_even || is_inexact_lt_midpoint) {
	Cstar.w[0] = Cstar.w[0] + 1;
      } else {
	;	// the result is already correct
      }
      res = Cstar.w[0];
    } else if (exp == 0) {
      // 1 <= q <= 10
      // res = +C (exact)
      res = C1.w[0];
    } else {	// if (exp > 0) => 1 <= exp <= 9, 1 <= q < 9, 2 <= q + exp <= 10
      // res = +C * 10^exp (exact)
      res = C1.w[0] * ten2k64[exp];
    }
  }
}

BID_RETURN (res);
}

/*****************************************************************************
 *  BID128_to_uint32_int
 ****************************************************************************/

BID128_FUNCTION_ARG1_NORND_CUSTOMRESTYPE (unsigned int,
					  bid128_to_uint32_int, x)

     int res;
     UINT64 x_sign;
     UINT64 x_exp;
     int exp;			// unbiased exponent
  // Note: C1.w[1], C1.w[0] represent x_signif_hi, x_signif_lo (all are UINT64)
     UINT64 tmp64, tmp64A;
     BID_UI64DOUBLE tmp1;
     unsigned int x_nr_bits;
     int q, ind, shift;
     UINT128 C1, C;
     UINT128 Cstar;		// C* represents up to 34 decimal digits ~ 113 bits
     UINT256 fstar;
     UINT256 P256;
     int is_inexact_gt_midpoint = 0;
     int is_midpoint_lt_even = 0;

  // unpack x
x_sign = x.w[1] & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
x_exp = x.w[1] & MASK_EXP;	// biased and shifted left 49 bit positions
C1.w[1] = x.w[1] & MASK_COEFF;
C1.w[0] = x.w[0];

  // check for NaN or Infinity
if ((x.w[1] & MASK_SPECIAL) == MASK_SPECIAL) {
    // x is special
if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
  if ((x.w[1] & MASK_SNAN) == MASK_SNAN) {	// x is SNAN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is QNaN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
} else {	// x is not a NaN, so it must be infinity
  if (!x_sign) {	// x is +inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is -inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
}
}
  // check for non-canonical values (after the check for special values)
if ((C1.w[1] > 0x0001ed09bead87c0ull)
    || (C1.w[1] == 0x0001ed09bead87c0ull
	&& (C1.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull)) {
  res = 0x00000000;
  BID_RETURN (res);
} else if ((C1.w[1] == 0x0ull) && (C1.w[0] == 0x0ull)) {
  // x is 0
  res = 0x00000000;
  BID_RETURN (res);
} else {	// x is not special and is not zero

  // q = nr. of decimal digits in x
  //  determine first the nr. of bits in x
  if (C1.w[1] == 0) {
    if (C1.w[0] >= 0x0020000000000000ull) {	// x >= 2^53
      // split the 64-bit value in two 32-bit halves to avoid rounding errors
      if (C1.w[0] >= 0x0000000100000000ull) {	// x >= 2^32
	tmp1.d = (double) (C1.w[0] >> 32);	// exact conversion
	x_nr_bits =
	  33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      } else {	// x < 2^32
	tmp1.d = (double) (C1.w[0]);	// exact conversion
	x_nr_bits =
	  1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      }
    } else {	// if x < 2^53
      tmp1.d = (double) C1.w[0];	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// C1.w[1] != 0 => nr. bits = 64 + nr_bits (C1.w[1])
    tmp1.d = (double) C1.w[1];	// exact conversion
    x_nr_bits =
      65 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1.w[1] > nr_digits[x_nr_bits - 1].threshold_hi
	|| (C1.w[1] == nr_digits[x_nr_bits - 1].threshold_hi
	    && C1.w[0] >= nr_digits[x_nr_bits - 1].threshold_lo))
      q++;
  }
  exp = (x_exp >> 49) - 6176;
  if ((q + exp) > 10) {	// x >= 10^10 ~= 2^33.2... (cannot fit in 32 bits)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
    BID_RETURN (res);
  } else if ((q + exp) == 10) {	// x = c(0)c(1)...c(9).c(10)...c(q-1)
    // in this case 2^29.89... ~= 10^9 <= x < 10^10 ~= 2^33.2...
    // so x rounded to an integer may or may not fit in a signed 32-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 10'
    if (x_sign) {	// if n < 0 and q + exp = 10
      // if n <= -1 then n is too large
      // too large if c(0)c(1)...c(9).c(10)...c(q-1) >= 1
      // <=> 0.c(0)c(1)...c(q-1) * 10^11 >= 0x0a, 1<=q<=34
      if (q <= 11) {
	tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
	// c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
	if (tmp64 >= 0x0aull) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit uint fall through
	// to '1 <= q + exp <= 10'
      } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
	// 0.c(0)c(1)...c(q-1) * 10^11 >= 0x0a <=>
	// C >= 0x0a * 10^(q-11) where 1 <= q - 11 <= 23
	// (scale 1 up)
	tmp64 = 0x0aull;
	if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	  __mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
	} else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	  __mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
	}
	if (C1.w[1] > C.w[1]
	    || (C1.w[1] == C.w[1] && C1.w[0] >= C.w[0])) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      }
    } else {	// if n > 0 and q + exp = 10
      // if n >= 2^32 then n is too large
      // too large if c(0)c(1)...c(9).c(10)...c(q-1) >= 2^32
      // too large if 0.c(0)c(1)...c(q-1) * 10^11 >= 0xa00000000, 1<=q<=34
      if (q <= 11) {
	tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
	// c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
	if (tmp64 >= 0xa00000000ull) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit uint fall through
	// to '1 <= q + exp <= 10'
      } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
	// 0.c(0)c(1)...c(q-1) * 10^11 >= 0xa00000000 <=>
	// C >= 0xa00000000 * 10^(q-11) where 1 <= q - 11 <= 23
	// (scale 2^32 up)
	tmp64 = 0xa00000000ull;
	if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	  __mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
	} else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	  __mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
	}
	if (C1.w[1] > C.w[1]
	    || (C1.w[1] == C.w[1] && C1.w[0] >= C.w[0])) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      }
    }
  }
  // n is not too large to be converted to uint32: -2^32 < n < 2^32
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) <= 0) {
    // n = +/-0.0...c(0)c(1)...c(q-1) or n = +/-0.c(0)c(1)...c(q-1)
    // return 0
    res = 0x00000000;
    BID_RETURN (res);
  } else {	// if (1 <= q + exp <= 10, 1 <= q <= 34, -33 <= exp <= 9)
    // x = d(0)...d(k).d(k+1)..., k >= 0, d(0) != 0
    if (x_sign) {	// x <= -1
      // set invalid flag
      *pfpsf |= INVALID_EXCEPTION;
      // return Integer Indefinite
      res = 0x80000000;
      BID_RETURN (res);
    }
    // x > 0 from this point on
    // 1 <= x < 2^32 so x can be rounded to zero to a 32-bit unsigned integer
    if (exp < 0) {	// 2 <= q <= 34, -33 <= exp <= -1, 1 <= q + exp <= 10
      ind = -exp;	// 1 <= ind <= 33; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 = C1 + 1/2 * 10^ind where the result C1 fits in 127 bits
      tmp64 = C1.w[0];
      if (ind <= 19) {
	C1.w[0] = C1.w[0] + midpoint64[ind - 1];
      } else {
	C1.w[0] = C1.w[0] + midpoint128[ind - 20].w[0];
	C1.w[1] = C1.w[1] + midpoint128[ind - 20].w[1];
      }
      if (C1.w[0] < tmp64)
	C1.w[1]++;
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 33
      // kx = 10^(-x) = ten2mk128[ind - 1]
      // C* = (C1 + 1/2 * 10^x) * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 118 bits
      __mul_128x128_to_256 (P256, C1, ten2mk128[ind - 1]);
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[1] = P256.w[3];
	Cstar.w[0] = P256.w[2];
	fstar.w[3] = 0;
	fstar.w[2] = P256.w[2] & maskhigh128[ind - 1];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[1] = 0;
	Cstar.w[0] = P256.w[3];
	fstar.w[3] = P256.w[3] & maskhigh128[ind - 1];
	fstar.w[2] = P256.w[2];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      }
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind], e.g.
      // if x=1, T*=ten2mk128trunc[0]=0x19999999999999999999999999999999
      // if (0 < f* < 10^(-x)) then the result is a midpoint
      //   if floor(C*) is even then C* = floor(C*) - logical right
      //       shift; C* has p decimal digits, correct by Prop. 1)
      //   else if floor(C*) is odd C* = floor(C*)-1 (logical right
      //       shift; C* has p decimal digits, correct by Pr. 1)
      // else
      //   C* = floor(C*) (logical right shift; C has p decimal digits,
      //       correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-128 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 102
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[0] =
	  (Cstar.w[0] >> shift) | (Cstar.w[1] << (64 - shift));
	// redundant, it will be 0! Cstar.w[1] = (Cstar.w[1] >> shift);
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[0] = (Cstar.w[0] >> (shift - 64));	// 2 <= shift - 64 <= 38
      }
      // determine inexactness of the rounding of C*
      // if (0 < f* - 1/2 < 10^(-x)) then
      //   the result is exact
      // else // if (f* - 1/2 > T*) then
      //   the result is inexact
      if (ind - 1 <= 2) {
	if (fstar.w[1] > 0x8000000000000000ull ||
	    (fstar.w[1] == 0x8000000000000000ull
	     && fstar.w[0] > 0x0ull)) {
	  // f* > 1/2 and the result may be exact
	  tmp64 = fstar.w[1] - 0x8000000000000000ull;	// f* - 1/2
	  if (tmp64 > ten2mk128trunc[ind - 1].w[1]
	      || (tmp64 == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] >= ten2mk128trunc[ind - 1].w[0])) {
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  is_inexact_gt_midpoint = 1;
	}
      } else if (ind - 1 <= 21) {	// if 3 <= ind <= 21
	if (fstar.w[3] > 0x0 ||
	    (fstar.w[3] == 0x0 && fstar.w[2] > onehalf128[ind - 1]) ||
	    (fstar.w[3] == 0x0 && fstar.w[2] == onehalf128[ind - 1] &&
	     (fstar.w[1] || fstar.w[0]))) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[2] - onehalf128[ind - 1];
	  tmp64A = fstar.w[3];
	  if (tmp64 > fstar.w[2])
	    tmp64A--;
	  if (tmp64A || tmp64
	      || fstar.w[1] > ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] > ten2mk128trunc[ind - 1].w[0])) {
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  is_inexact_gt_midpoint = 1;
	}
      } else {	// if 22 <= ind <= 33
	if (fstar.w[3] > onehalf128[ind - 1] ||
	    (fstar.w[3] == onehalf128[ind - 1] &&
	     (fstar.w[2] || fstar.w[1] || fstar.w[0]))) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[3] - onehalf128[ind - 1];
	  if (tmp64 || fstar.w[2]
	      || fstar.w[1] > ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] > ten2mk128trunc[ind - 1].w[0])) {
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  is_inexact_gt_midpoint = 1;
	}
      }

      // if the result was a midpoint it was rounded away from zero, so
      // it will need a correction
      // check for midpoints
      if ((fstar.w[3] == 0) && (fstar.w[2] == 0)
	  && (fstar.w[1] || fstar.w[0])
	  && (fstar.w[1] < ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] <= ten2mk128trunc[ind - 1].w[0]))) {
	// the result is a midpoint; round to nearest
	if (Cstar.w[0] & 0x01) {	// Cstar.w[0] is odd; MP in [EVEN, ODD]
	  // if floor(C*) is odd C = floor(C*) - 1; the result >= 1
	  Cstar.w[0]--;	// Cstar.w[0] is now even
	  is_inexact_gt_midpoint = 0;
	} else {	// else MP in [ODD, EVEN]
	  is_midpoint_lt_even = 1;
	  is_inexact_gt_midpoint = 0;
	}
      }
      // general correction for RZ
      if (is_midpoint_lt_even || is_inexact_gt_midpoint) {
	Cstar.w[0] = Cstar.w[0] - 1;
      } else {
	;	// exact, the result is already correct
      }
      res = Cstar.w[0];
    } else if (exp == 0) {
      // 1 <= q <= 10
      // res = +C (exact)
      res = C1.w[0];
    } else {	// if (exp > 0) => 1 <= exp <= 9, 1 <= q < 9, 2 <= q + exp <= 10
      // res = +C * 10^exp (exact)
      res = C1.w[0] * ten2k64[exp];
    }
  }
}

BID_RETURN (res);
}

/*****************************************************************************
 *  BID128_to_uint32_xint
 ****************************************************************************/

BID128_FUNCTION_ARG1_NORND_CUSTOMRESTYPE (unsigned int,
					  bid128_to_uint32_xint, x)

     int res;
     UINT64 x_sign;
     UINT64 x_exp;
     int exp;			// unbiased exponent
  // Note: C1.w[1], C1.w[0] represent x_signif_hi, x_signif_lo (all are UINT64)
     UINT64 tmp64, tmp64A;
     BID_UI64DOUBLE tmp1;
     unsigned int x_nr_bits;
     int q, ind, shift;
     UINT128 C1, C;
     UINT128 Cstar;		// C* represents up to 34 decimal digits ~ 113 bits
     UINT256 fstar;
     UINT256 P256;
     int is_inexact_gt_midpoint = 0;
     int is_midpoint_lt_even = 0;

  // unpack x
x_sign = x.w[1] & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
x_exp = x.w[1] & MASK_EXP;	// biased and shifted left 49 bit positions
C1.w[1] = x.w[1] & MASK_COEFF;
C1.w[0] = x.w[0];

  // check for NaN or Infinity
if ((x.w[1] & MASK_SPECIAL) == MASK_SPECIAL) {
    // x is special
if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
  if ((x.w[1] & MASK_SNAN) == MASK_SNAN) {	// x is SNAN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is QNaN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
} else {	// x is not a NaN, so it must be infinity
  if (!x_sign) {	// x is +inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is -inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
}
}
  // check for non-canonical values (after the check for special values)
if ((C1.w[1] > 0x0001ed09bead87c0ull)
    || (C1.w[1] == 0x0001ed09bead87c0ull
	&& (C1.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull)) {
  res = 0x00000000;
  BID_RETURN (res);
} else if ((C1.w[1] == 0x0ull) && (C1.w[0] == 0x0ull)) {
  // x is 0
  res = 0x00000000;
  BID_RETURN (res);
} else {	// x is not special and is not zero

  // q = nr. of decimal digits in x
  //  determine first the nr. of bits in x
  if (C1.w[1] == 0) {
    if (C1.w[0] >= 0x0020000000000000ull) {	// x >= 2^53
      // split the 64-bit value in two 32-bit halves to avoid rounding errors
      if (C1.w[0] >= 0x0000000100000000ull) {	// x >= 2^32
	tmp1.d = (double) (C1.w[0] >> 32);	// exact conversion
	x_nr_bits =
	  33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      } else {	// x < 2^32
	tmp1.d = (double) (C1.w[0]);	// exact conversion
	x_nr_bits =
	  1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      }
    } else {	// if x < 2^53
      tmp1.d = (double) C1.w[0];	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// C1.w[1] != 0 => nr. bits = 64 + nr_bits (C1.w[1])
    tmp1.d = (double) C1.w[1];	// exact conversion
    x_nr_bits =
      65 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1.w[1] > nr_digits[x_nr_bits - 1].threshold_hi
	|| (C1.w[1] == nr_digits[x_nr_bits - 1].threshold_hi
	    && C1.w[0] >= nr_digits[x_nr_bits - 1].threshold_lo))
      q++;
  }
  exp = (x_exp >> 49) - 6176;
  if ((q + exp) > 10) {	// x >= 10^10 ~= 2^33.2... (cannot fit in 32 bits)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
    BID_RETURN (res);
  } else if ((q + exp) == 10) {	// x = c(0)c(1)...c(9).c(10)...c(q-1)
    // in this case 2^29.89... ~= 10^9 <= x < 10^10 ~= 2^33.2...
    // so x rounded to an integer may or may not fit in a signed 32-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 10'
    if (x_sign) {	// if n < 0 and q + exp = 10
      // if n <= -1 then n is too large
      // too large if c(0)c(1)...c(9).c(10)...c(q-1) >= 1
      // <=> 0.c(0)c(1)...c(q-1) * 10^11 >= 0x0a, 1<=q<=34
      if (q <= 11) {
	tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
	// c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
	if (tmp64 >= 0x0aull) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit uint fall through
	// to '1 <= q + exp <= 10'
      } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
	// 0.c(0)c(1)...c(q-1) * 10^11 >= 0x0a <=>
	// C >= 0x0a * 10^(q-11) where 1 <= q - 11 <= 23
	// (scale 1 up)
	tmp64 = 0x0aull;
	if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	  __mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
	} else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	  __mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
	}
	if (C1.w[1] > C.w[1]
	    || (C1.w[1] == C.w[1] && C1.w[0] >= C.w[0])) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      }
    } else {	// if n > 0 and q + exp = 10
      // if n >= 2^32 then n is too large
      // too large if c(0)c(1)...c(9).c(10)...c(q-1) >= 2^32
      // too large if 0.c(0)c(1)...c(q-1) * 10^11 >= 0xa00000000, 1<=q<=34
      if (q <= 11) {
	tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
	// c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
	if (tmp64 >= 0xa00000000ull) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit uint fall through
	// to '1 <= q + exp <= 10'
      } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
	// 0.c(0)c(1)...c(q-1) * 10^11 >= 0xa00000000 <=>
	// C >= 0xa00000000 * 10^(q-11) where 1 <= q - 11 <= 23
	// (scale 2^32 up)
	tmp64 = 0xa00000000ull;
	if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	  __mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
	} else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	  __mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
	}
	if (C1.w[1] > C.w[1]
	    || (C1.w[1] == C.w[1] && C1.w[0] >= C.w[0])) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      }
    }
  }
  // n is not too large to be converted to uint32: -2^32 < n < 2^32
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) <= 0) {
    // n = +/-0.0...c(0)c(1)...c(q-1) or n = +/-0.c(0)c(1)...c(q-1)
    // set inexact flag
    *pfpsf |= INEXACT_EXCEPTION;
    // return 0
    res = 0x00000000;
    BID_RETURN (res);
  } else {	// if (1 <= q + exp <= 10, 1 <= q <= 34, -33 <= exp <= 9)
    // x = d(0)...d(k).d(k+1)..., k >= 0, d(0) != 0
    if (x_sign) {	// x <= -1
      // set invalid flag
      *pfpsf |= INVALID_EXCEPTION;
      // return Integer Indefinite
      res = 0x80000000;
      BID_RETURN (res);
    }
    // x > 0 from this point on
    // 1 <= x < 2^32 so x can be rounded to zero to a 32-bit unsigned integer
    if (exp < 0) {	// 2 <= q <= 34, -33 <= exp <= -1, 1 <= q + exp <= 10
      ind = -exp;	// 1 <= ind <= 33; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 = C1 + 1/2 * 10^ind where the result C1 fits in 127 bits
      tmp64 = C1.w[0];
      if (ind <= 19) {
	C1.w[0] = C1.w[0] + midpoint64[ind - 1];
      } else {
	C1.w[0] = C1.w[0] + midpoint128[ind - 20].w[0];
	C1.w[1] = C1.w[1] + midpoint128[ind - 20].w[1];
      }
      if (C1.w[0] < tmp64)
	C1.w[1]++;
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 33
      // kx = 10^(-x) = ten2mk128[ind - 1]
      // C* = (C1 + 1/2 * 10^x) * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 118 bits
      __mul_128x128_to_256 (P256, C1, ten2mk128[ind - 1]);
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[1] = P256.w[3];
	Cstar.w[0] = P256.w[2];
	fstar.w[3] = 0;
	fstar.w[2] = P256.w[2] & maskhigh128[ind - 1];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[1] = 0;
	Cstar.w[0] = P256.w[3];
	fstar.w[3] = P256.w[3] & maskhigh128[ind - 1];
	fstar.w[2] = P256.w[2];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      }
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind], e.g.
      // if x=1, T*=ten2mk128trunc[0]=0x19999999999999999999999999999999
      // if (0 < f* < 10^(-x)) then the result is a midpoint
      //   if floor(C*) is even then C* = floor(C*) - logical right
      //       shift; C* has p decimal digits, correct by Prop. 1)
      //   else if floor(C*) is odd C* = floor(C*)-1 (logical right
      //       shift; C* has p decimal digits, correct by Pr. 1)
      // else
      //   C* = floor(C*) (logical right shift; C has p decimal digits,
      //       correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-128 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 102
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[0] =
	  (Cstar.w[0] >> shift) | (Cstar.w[1] << (64 - shift));
	// redundant, it will be 0! Cstar.w[1] = (Cstar.w[1] >> shift);
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[0] = (Cstar.w[0] >> (shift - 64));	// 2 <= shift - 64 <= 38
      }
      // determine inexactness of the rounding of C*
      // if (0 < f* - 1/2 < 10^(-x)) then
      //   the result is exact
      // else // if (f* - 1/2 > T*) then
      //   the result is inexact
      if (ind - 1 <= 2) {
	if (fstar.w[1] > 0x8000000000000000ull ||
	    (fstar.w[1] == 0x8000000000000000ull
	     && fstar.w[0] > 0x0ull)) {
	  // f* > 1/2 and the result may be exact
	  tmp64 = fstar.w[1] - 0x8000000000000000ull;	// f* - 1/2
	  if (tmp64 > ten2mk128trunc[ind - 1].w[1]
	      || (tmp64 == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] >= ten2mk128trunc[ind - 1].w[0])) {
	    // set the inexact flag
	    *pfpsf |= INEXACT_EXCEPTION;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	  is_inexact_gt_midpoint = 1;
	}
      } else if (ind - 1 <= 21) {	// if 3 <= ind <= 21
	if (fstar.w[3] > 0x0 ||
	    (fstar.w[3] == 0x0 && fstar.w[2] > onehalf128[ind - 1]) ||
	    (fstar.w[3] == 0x0 && fstar.w[2] == onehalf128[ind - 1] &&
	     (fstar.w[1] || fstar.w[0]))) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[2] - onehalf128[ind - 1];
	  tmp64A = fstar.w[3];
	  if (tmp64 > fstar.w[2])
	    tmp64A--;
	  if (tmp64A || tmp64
	      || fstar.w[1] > ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] > ten2mk128trunc[ind - 1].w[0])) {
	    // set the inexact flag
	    *pfpsf |= INEXACT_EXCEPTION;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	  is_inexact_gt_midpoint = 1;
	}
      } else {	// if 22 <= ind <= 33
	if (fstar.w[3] > onehalf128[ind - 1] ||
	    (fstar.w[3] == onehalf128[ind - 1] &&
	     (fstar.w[2] || fstar.w[1] || fstar.w[0]))) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[3] - onehalf128[ind - 1];
	  if (tmp64 || fstar.w[2]
	      || fstar.w[1] > ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] > ten2mk128trunc[ind - 1].w[0])) {
	    // set the inexact flag
	    *pfpsf |= INEXACT_EXCEPTION;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	  is_inexact_gt_midpoint = 1;
	}
      }

      // if the result was a midpoint it was rounded away from zero, so
      // it will need a correction
      // check for midpoints
      if ((fstar.w[3] == 0) && (fstar.w[2] == 0)
	  && (fstar.w[1] || fstar.w[0])
	  && (fstar.w[1] < ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] <= ten2mk128trunc[ind - 1].w[0]))) {
	// the result is a midpoint; round to nearest
	if (Cstar.w[0] & 0x01) {	// Cstar.w[0] is odd; MP in [EVEN, ODD]
	  // if floor(C*) is odd C = floor(C*) - 1; the result >= 1
	  Cstar.w[0]--;	// Cstar.w[0] is now even
	  is_inexact_gt_midpoint = 0;
	} else {	// else MP in [ODD, EVEN]
	  is_midpoint_lt_even = 1;
	  is_inexact_gt_midpoint = 0;
	}
      }
      // general correction for RZ
      if (is_midpoint_lt_even || is_inexact_gt_midpoint) {
	Cstar.w[0] = Cstar.w[0] - 1;
      } else {
	;	// exact, the result is already correct
      }
      res = Cstar.w[0];
    } else if (exp == 0) {
      // 1 <= q <= 10
      // res = +C (exact)
      res = C1.w[0];
    } else {	// if (exp > 0) => 1 <= exp <= 9, 1 <= q < 9, 2 <= q + exp <= 10
      // res = +C * 10^exp (exact)
      res = C1.w[0] * ten2k64[exp];
    }
  }
}

BID_RETURN (res);
}

/*****************************************************************************
 *  BID128_to_uint32_rninta
 ****************************************************************************/

BID128_FUNCTION_ARG1_NORND_CUSTOMRESTYPE (unsigned int,
					  bid128_to_uint32_rninta, x)

     unsigned int res;
     UINT64 x_sign;
     UINT64 x_exp;
     int exp;			// unbiased exponent
  // Note: C1.w[1], C1.w[0] represent x_signif_hi, x_signif_lo (all are UINT64)
     UINT64 tmp64;
     BID_UI64DOUBLE tmp1;
     unsigned int x_nr_bits;
     int q, ind, shift;
     UINT128 C1, C;
     UINT128 Cstar;		// C* represents up to 34 decimal digits ~ 113 bits
     UINT256 P256;

  // unpack x
x_sign = x.w[1] & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
x_exp = x.w[1] & MASK_EXP;	// biased and shifted left 49 bit positions
C1.w[1] = x.w[1] & MASK_COEFF;
C1.w[0] = x.w[0];

  // check for NaN or Infinity
if ((x.w[1] & MASK_SPECIAL) == MASK_SPECIAL) {
    // x is special
if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
  if ((x.w[1] & MASK_SNAN) == MASK_SNAN) {	// x is SNAN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is QNaN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
} else {	// x is not a NaN, so it must be infinity
  if (!x_sign) {	// x is +inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is -inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
}
}
  // check for non-canonical values (after the check for special values)
if ((C1.w[1] > 0x0001ed09bead87c0ull)
    || (C1.w[1] == 0x0001ed09bead87c0ull
	&& (C1.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull)) {
  res = 0x00000000;
  BID_RETURN (res);
} else if ((C1.w[1] == 0x0ull) && (C1.w[0] == 0x0ull)) {
  // x is 0
  res = 0x00000000;
  BID_RETURN (res);
} else {	// x is not special and is not zero

  // q = nr. of decimal digits in x
  //  determine first the nr. of bits in x
  if (C1.w[1] == 0) {
    if (C1.w[0] >= 0x0020000000000000ull) {	// x >= 2^53
      // split the 64-bit value in two 32-bit halves to avoid rounding errors
      if (C1.w[0] >= 0x0000000100000000ull) {	// x >= 2^32
	tmp1.d = (double) (C1.w[0] >> 32);	// exact conversion
	x_nr_bits =
	  33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      } else {	// x < 2^32
	tmp1.d = (double) (C1.w[0]);	// exact conversion
	x_nr_bits =
	  1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      }
    } else {	// if x < 2^53
      tmp1.d = (double) C1.w[0];	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// C1.w[1] != 0 => nr. bits = 64 + nr_bits (C1.w[1])
    tmp1.d = (double) C1.w[1];	// exact conversion
    x_nr_bits =
      65 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1.w[1] > nr_digits[x_nr_bits - 1].threshold_hi
	|| (C1.w[1] == nr_digits[x_nr_bits - 1].threshold_hi
	    && C1.w[0] >= nr_digits[x_nr_bits - 1].threshold_lo))
      q++;
  }
  exp = (x_exp >> 49) - 6176;
  if ((q + exp) > 10) {	// x >= 10^10 ~= 2^33.2... (cannot fit in 32 bits)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
    BID_RETURN (res);
  } else if ((q + exp) == 10) {	// x = c(0)c(1)...c(9).c(10)...c(q-1)
    // in this case 2^29.89... ~= 10^9 <= x < 10^10 ~= 2^33.2...
    // so x rounded to an integer may or may not fit in a signed 32-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 10'
    if (x_sign) {	// if n < 0 and q + exp = 10
      // if n <= -1/2 then n is too large
      // too large if c(0)c(1)...c(9).c(10)...c(q-1) >= 1/2
      // <=> 0.c(0)c(1)...c(q-1) * 10^11 >= 0x05, 1<=q<=34
      if (q <= 11) {
	tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
	// c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
	if (tmp64 >= 0x05ull) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
	// 0.c(0)c(1)...c(q-1) * 10^11 >= 0x05 <=>
	// C >= 0x05 * 10^(q-11) where 1 <= q - 11 <= 23
	// (scale 1/2 up)
	tmp64 = 0x05ull;
	if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	  __mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
	} else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	  __mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
	}
	if (C1.w[1] > C.w[1]
	    || (C1.w[1] == C.w[1] && C1.w[0] >= C.w[0])) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      }
    } else {	// if n > 0 and q + exp = 10
      // if n >= 2^32 - 1/2 then n is too large
      // too large if c(0)c(1)...c(9).c(10)...c(q-1) >= 2^32-1/2
      // too large if 0.c(0)c(1)...c(q-1) * 10^11 >= 0x9fffffffb, 1<=q<=34
      if (q <= 11) {
	tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
	// c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
	if (tmp64 >= 0x9fffffffbull) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
	// 0.c(0)c(1)...c(q-1) * 10^11 >= 0x9fffffffb <=>
	// C >= 0x9fffffffb * 10^(q-11) where 1 <= q - 11 <= 23
	// (scale 2^32-1/2 up)
	tmp64 = 0x9fffffffbull;
	if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	  __mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
	} else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	  __mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
	}
	if (C1.w[1] > C.w[1]
	    || (C1.w[1] == C.w[1] && C1.w[0] >= C.w[0])) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      }
    }
  }
  // n is not too large to be converted to int32: -1/2 < n < 2^32 - 1/2
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) < 0) {	// n = +/-0.0...c(0)c(1)...c(q-1)
    // return 0
    res = 0x00000000;
    BID_RETURN (res);
  } else if ((q + exp) == 0) {	// n = +/-0.c(0)c(1)...c(q-1)
    // if 0.c(0)c(1)...c(q-1) < 0.5 <=> c(0)c(1)...c(q-1) < 5 * 10^(q-1)
    //   res = 0
    // else if x > 0
    //   res = +1
    // else // if x < 0  
    //   invalid exc  
    ind = q - 1;
    if (ind <= 18) {	// 0 <= ind <= 18
      if ((C1.w[1] == 0) && (C1.w[0] < midpoint64[ind])) {
	res = 0x00000000;	// return 0
      } else if (!x_sign) {	// n > 0
	res = 0x00000001;	// return +1
      } else {
	res = 0x80000000;
	*pfpsf |= INVALID_EXCEPTION;
      }
    } else {	// 19 <= ind <= 33
      if ((C1.w[1] < midpoint128[ind - 19].w[1])
	  || ((C1.w[1] == midpoint128[ind - 19].w[1])
	      && (C1.w[0] < midpoint128[ind - 19].w[0]))) {
	res = 0x00000000;	// return 0
      } else if (!x_sign) {	// n > 0
	res = 0x00000001;	// return +1
      } else {
	res = 0x80000000;
	*pfpsf |= INVALID_EXCEPTION;
      }
    }
  } else {	// if (1 <= q + exp <= 10, 1 <= q <= 34, -33 <= exp <= 9)
    if (x_sign) {	// x <= -1
      // set invalid flag
      *pfpsf |= INVALID_EXCEPTION;
      // return Integer Indefinite
      res = 0x80000000;
      BID_RETURN (res);
    }
    // 1 <= x < 2^31-1/2 so x can be rounded
    // to nearest-away to a 32-bit signed integer
    if (exp < 0) {	// 2 <= q <= 34, -33 <= exp <= -1, 1 <= q + exp <= 10
      ind = -exp;	// 1 <= ind <= 33; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 = C1 + 1/2 * 10^ind where the result C1 fits in 127 bits
      tmp64 = C1.w[0];
      if (ind <= 19) {
	C1.w[0] = C1.w[0] + midpoint64[ind - 1];
      } else {
	C1.w[0] = C1.w[0] + midpoint128[ind - 20].w[0];
	C1.w[1] = C1.w[1] + midpoint128[ind - 20].w[1];
      }
      if (C1.w[0] < tmp64)
	C1.w[1]++;
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 33
      // kx = 10^(-x) = ten2mk128[ind - 1]
      // C* = (C1 + 1/2 * 10^x) * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 118 bits
      __mul_128x128_to_256 (P256, C1, ten2mk128[ind - 1]);
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[1] = P256.w[3];
	Cstar.w[0] = P256.w[2];
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[1] = 0;
	Cstar.w[0] = P256.w[3];
      }
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind], e.g.
      // if x=1, T*=ten2mk128trunc[0]=0x19999999999999999999999999999999
      // if (0 < f* < 10^(-x)) then the result is a midpoint
      //   if floor(C*) is even then C* = floor(C*) - logical right
      //       shift; C* has p decimal digits, correct by Prop. 1)
      //   else if floor(C*) is odd C* = floor(C*)-1 (logical right
      //       shift; C* has p decimal digits, correct by Pr. 1)
      // else
      //   C* = floor(C*) (logical right shift; C has p decimal digits,
      //       correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-128 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 102
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[0] =
	  (Cstar.w[0] >> shift) | (Cstar.w[1] << (64 - shift));
	// redundant, it will be 0! Cstar.w[1] = (Cstar.w[1] >> shift);
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[0] = (Cstar.w[0] >> (shift - 64));	// 2 <= shift - 64 <= 38
      }
      // if the result was a midpoint, it was already rounded away from zero
      res = Cstar.w[0];	// always positive
      // no need to check for midpoints - already rounded away from zero!
    } else if (exp == 0) {
      // 1 <= q <= 10
      // res = +C (exact)
      res = C1.w[0];
    } else {	// if (exp > 0) => 1 <= exp <= 9, 1 <= q < 9, 2 <= q + exp <= 10
      // res = +C * 10^exp (exact)
      res = C1.w[0] * ten2k64[exp];
    }
  }
}

BID_RETURN (res);
}

/*****************************************************************************
 *  BID128_to_uint32_xrninta
 ****************************************************************************/

BID128_FUNCTION_ARG1_NORND_CUSTOMRESTYPE (unsigned int,
					  bid128_to_uint32_xrninta, x)

     unsigned int res;
     UINT64 x_sign;
     UINT64 x_exp;
     int exp;			// unbiased exponent
  // Note: C1.w[1], C1.w[0] represent x_signif_hi, x_signif_lo (all are UINT64)
     UINT64 tmp64, tmp64A;
     BID_UI64DOUBLE tmp1;
     unsigned int x_nr_bits;
     int q, ind, shift;
     UINT128 C1, C;
     UINT128 Cstar;		// C* represents up to 34 decimal digits ~ 113 bits
     UINT256 fstar;
     UINT256 P256;
     unsigned int tmp_inexact = 0;

  // unpack x
x_sign = x.w[1] & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
x_exp = x.w[1] & MASK_EXP;	// biased and shifted left 49 bit positions
C1.w[1] = x.w[1] & MASK_COEFF;
C1.w[0] = x.w[0];

  // check for NaN or Infinity
if ((x.w[1] & MASK_SPECIAL) == MASK_SPECIAL) {
    // x is special
if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
  if ((x.w[1] & MASK_SNAN) == MASK_SNAN) {	// x is SNAN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is QNaN
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
} else {	// x is not a NaN, so it must be infinity
  if (!x_sign) {	// x is +inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  } else {	// x is -inf
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
  }
  BID_RETURN (res);
}
}
  // check for non-canonical values (after the check for special values)
if ((C1.w[1] > 0x0001ed09bead87c0ull)
    || (C1.w[1] == 0x0001ed09bead87c0ull
	&& (C1.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull)) {
  res = 0x00000000;
  BID_RETURN (res);
} else if ((C1.w[1] == 0x0ull) && (C1.w[0] == 0x0ull)) {
  // x is 0
  res = 0x00000000;
  BID_RETURN (res);
} else {	// x is not special and is not zero

  // q = nr. of decimal digits in x
  //  determine first the nr. of bits in x
  if (C1.w[1] == 0) {
    if (C1.w[0] >= 0x0020000000000000ull) {	// x >= 2^53
      // split the 64-bit value in two 32-bit halves to avoid rounding errors
      if (C1.w[0] >= 0x0000000100000000ull) {	// x >= 2^32
	tmp1.d = (double) (C1.w[0] >> 32);	// exact conversion
	x_nr_bits =
	  33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      } else {	// x < 2^32
	tmp1.d = (double) (C1.w[0]);	// exact conversion
	x_nr_bits =
	  1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
      }
    } else {	// if x < 2^53
      tmp1.d = (double) C1.w[0];	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// C1.w[1] != 0 => nr. bits = 64 + nr_bits (C1.w[1])
    tmp1.d = (double) C1.w[1];	// exact conversion
    x_nr_bits =
      65 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1.w[1] > nr_digits[x_nr_bits - 1].threshold_hi
	|| (C1.w[1] == nr_digits[x_nr_bits - 1].threshold_hi
	    && C1.w[0] >= nr_digits[x_nr_bits - 1].threshold_lo))
      q++;
  }
  exp = (x_exp >> 49) - 6176;
  if ((q + exp) > 10) {	// x >= 10^10 ~= 2^33.2... (cannot fit in 32 bits)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x80000000;
    BID_RETURN (res);
  } else if ((q + exp) == 10) {	// x = c(0)c(1)...c(9).c(10)...c(q-1)
    // in this case 2^29.89... ~= 10^9 <= x < 10^10 ~= 2^33.2...
    // so x rounded to an integer may or may not fit in a signed 32-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 10'
    if (x_sign) {	// if n < 0 and q + exp = 10
      // if n <= -1/2 then n is too large
      // too large if c(0)c(1)...c(9).c(10)...c(q-1) >= 1/2
      // <=> 0.c(0)c(1)...c(q-1) * 10^11 >= 0x05, 1<=q<=34
      if (q <= 11) {
	tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
	// c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
	if (tmp64 >= 0x05ull) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
	// 0.c(0)c(1)...c(q-1) * 10^11 >= 0x05 <=>
	// C >= 0x05 * 10^(q-11) where 1 <= q - 11 <= 23
	// (scale 1/2 up)
	tmp64 = 0x05ull;
	if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	  __mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
	} else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	  __mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
	}
	if (C1.w[1] > C.w[1]
	    || (C1.w[1] == C.w[1] && C1.w[0] >= C.w[0])) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      }
    } else {	// if n > 0 and q + exp = 10
      // if n >= 2^32 - 1/2 then n is too large
      // too large if c(0)c(1)...c(9).c(10)...c(q-1) >= 2^32-1/2
      // too large if 0.c(0)c(1)...c(q-1) * 10^11 >= 0x9fffffffb, 1<=q<=34
      if (q <= 11) {
	tmp64 = C1.w[0] * ten2k64[11 - q];	// C scaled up to 11-digit int
	// c(0)c(1)...c(9)c(10) or c(0)c(1)...c(q-1)0...0 (11 digits)
	if (tmp64 >= 0x9fffffffbull) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      } else {	// if (q > 11), i.e. 12 <= q <= 34 and so -24 <= exp <= -2
	// 0.c(0)c(1)...c(q-1) * 10^11 >= 0x9fffffffb <=>
	// C >= 0x9fffffffb * 10^(q-11) where 1 <= q - 11 <= 23
	// (scale 2^32-1/2 up)
	tmp64 = 0x9fffffffbull;
	if (q - 11 <= 19) {	// 1 <= q - 11 <= 19; 10^(q-11) requires 64 bits
	  __mul_64x64_to_128MACH (C, tmp64, ten2k64[q - 11]);
	} else {	// 20 <= q - 11 <= 23, and 10^(q-11) requires 128 bits
	  __mul_128x64_to_128 (C, tmp64, ten2k128[q - 31]);
	}
	if (C1.w[1] > C.w[1]
	    || (C1.w[1] == C.w[1] && C1.w[0] >= C.w[0])) {
	  // set invalid flag
	  *pfpsf |= INVALID_EXCEPTION;
	  // return Integer Indefinite
	  res = 0x80000000;
	  BID_RETURN (res);
	}
	// else cases that can be rounded to a 32-bit int fall through
	// to '1 <= q + exp <= 10'
      }
    }
  }
  // n is not too large to be converted to int32: -1/2 < n < 2^32 - 1/2
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) < 0) {	// n = +/-0.0...c(0)c(1)...c(q-1)
    // set inexact flag
    *pfpsf |= INEXACT_EXCEPTION;
    // return 0
    res = 0x00000000;
    BID_RETURN (res);
  } else if ((q + exp) == 0) {	// n = +/-0.c(0)c(1)...c(q-1)
    // if 0.c(0)c(1)...c(q-1) < 0.5 <=> c(0)c(1)...c(q-1) < 5 * 10^(q-1)
    //   res = 0
    // else if x > 0
    //   res = +1
    // else // if x < 0  
    //   invalid exc  
    ind = q - 1;
    if (ind <= 18) {	// 0 <= ind <= 18
      if ((C1.w[1] == 0) && (C1.w[0] < midpoint64[ind])) {
	res = 0x00000000;	// return 0
      } else if (!x_sign) {	// n > 0
	res = 0x00000001;	// return +1
      } else {
	res = 0x80000000;
	*pfpsf |= INVALID_EXCEPTION;
	BID_RETURN (res);
      }
    } else {	// 19 <= ind <= 33
      if ((C1.w[1] < midpoint128[ind - 19].w[1])
	  || ((C1.w[1] == midpoint128[ind - 19].w[1])
	      && (C1.w[0] < midpoint128[ind - 19].w[0]))) {
	res = 0x00000000;	// return 0
      } else if (!x_sign) {	// n > 0
	res = 0x00000001;	// return +1
      } else {
	res = 0x80000000;
	*pfpsf |= INVALID_EXCEPTION;
	BID_RETURN (res);
      }
    }
    // set inexact flag
    *pfpsf |= INEXACT_EXCEPTION;
  } else {	// if (1 <= q + exp <= 10, 1 <= q <= 34, -33 <= exp <= 9)
    if (x_sign) {	// x <= -1
      // set invalid flag
      *pfpsf |= INVALID_EXCEPTION;
      // return Integer Indefinite
      res = 0x80000000;
      BID_RETURN (res);
    }
    // 1 <= x < 2^31-1/2 so x can be rounded
    // to nearest-away to a 32-bit signed integer
    if (exp < 0) {	// 2 <= q <= 34, -33 <= exp <= -1, 1 <= q + exp <= 10
      ind = -exp;	// 1 <= ind <= 33; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 = C1 + 1/2 * 10^ind where the result C1 fits in 127 bits
      tmp64 = C1.w[0];
      if (ind <= 19) {
	C1.w[0] = C1.w[0] + midpoint64[ind - 1];
      } else {
	C1.w[0] = C1.w[0] + midpoint128[ind - 20].w[0];
	C1.w[1] = C1.w[1] + midpoint128[ind - 20].w[1];
      }
      if (C1.w[0] < tmp64)
	C1.w[1]++;
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 33
      // kx = 10^(-x) = ten2mk128[ind - 1]
      // C* = (C1 + 1/2 * 10^x) * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 118 bits
      __mul_128x128_to_256 (P256, C1, ten2mk128[ind - 1]);
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[1] = P256.w[3];
	Cstar.w[0] = P256.w[2];
	fstar.w[3] = 0;
	fstar.w[2] = P256.w[2] & maskhigh128[ind - 1];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[1] = 0;
	Cstar.w[0] = P256.w[3];
	fstar.w[3] = P256.w[3] & maskhigh128[ind - 1];
	fstar.w[2] = P256.w[2];
	fstar.w[1] = P256.w[1];
	fstar.w[0] = P256.w[0];
      }
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind], e.g.
      // if x=1, T*=ten2mk128trunc[0]=0x19999999999999999999999999999999
      // if (0 < f* < 10^(-x)) then the result is a midpoint
      //   if floor(C*) is even then C* = floor(C*) - logical right
      //       shift; C* has p decimal digits, correct by Prop. 1)
      //   else if floor(C*) is odd C* = floor(C*)-1 (logical right
      //       shift; C* has p decimal digits, correct by Pr. 1)
      // else
      //   C* = floor(C*) (logical right shift; C has p decimal digits,
      //       correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-128 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 102
      if (ind - 1 <= 21) {	// 0 <= ind - 1 <= 21
	Cstar.w[0] =
	  (Cstar.w[0] >> shift) | (Cstar.w[1] << (64 - shift));
	// redundant, it will be 0! Cstar.w[1] = (Cstar.w[1] >> shift);
      } else {	// 22 <= ind - 1 <= 33
	Cstar.w[0] = (Cstar.w[0] >> (shift - 64));	// 2 <= shift - 64 <= 38
      }
      // if the result was a midpoint, it was already rounded away from zero
      // determine inexactness of the rounding of C*
      // if (0 < f* - 1/2 < 10^(-x)) then
      //   the result is exact
      // else // if (f* - 1/2 > T*) then
      //   the result is inexact
      if (ind - 1 <= 2) {
	if (fstar.w[1] > 0x8000000000000000ull ||
	    (fstar.w[1] == 0x8000000000000000ull
	     && fstar.w[0] > 0x0ull)) {
	  // f* > 1/2 and the result may be exact
	  tmp64 = fstar.w[1] - 0x8000000000000000ull;	// f* - 1/2
	  if (tmp64 > ten2mk128trunc[ind - 1].w[1]
	      || (tmp64 == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] >= ten2mk128trunc[ind - 1].w[0])) {
	    // set the inexact flag
	    // *pfpsf |= INEXACT_EXCEPTION;
	    tmp_inexact = 1;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  // *pfpsf |= INEXACT_EXCEPTION;
	  tmp_inexact = 1;
	}
      } else if (ind - 1 <= 21) {	// if 3 <= ind <= 21
	if (fstar.w[3] > 0x0 ||
	    (fstar.w[3] == 0x0 && fstar.w[2] > onehalf128[ind - 1]) ||
	    (fstar.w[3] == 0x0 && fstar.w[2] == onehalf128[ind - 1] &&
	     (fstar.w[1] || fstar.w[0]))) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[2] - onehalf128[ind - 1];
	  tmp64A = fstar.w[3];
	  if (tmp64 > fstar.w[2])
	    tmp64A--;
	  if (tmp64A || tmp64
	      || fstar.w[1] > ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] > ten2mk128trunc[ind - 1].w[0])) {
	    // set the inexact flag
	    // *pfpsf |= INEXACT_EXCEPTION;
	    tmp_inexact = 1;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  // *pfpsf |= INEXACT_EXCEPTION;
	  tmp_inexact = 1;
	}
      } else {	// if 22 <= ind <= 33
	if (fstar.w[3] > onehalf128[ind - 1] ||
	    (fstar.w[3] == onehalf128[ind - 1] &&
	     (fstar.w[2] || fstar.w[1] || fstar.w[0]))) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[3] - onehalf128[ind - 1];
	  if (tmp64 || fstar.w[2]
	      || fstar.w[1] > ten2mk128trunc[ind - 1].w[1]
	      || (fstar.w[1] == ten2mk128trunc[ind - 1].w[1]
		  && fstar.w[0] > ten2mk128trunc[ind - 1].w[0])) {
	    // set the inexact flag
	    // *pfpsf |= INEXACT_EXCEPTION;
	    tmp_inexact = 1;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  // *pfpsf |= INEXACT_EXCEPTION;
	  tmp_inexact = 1;
	}
      }
      // no need to check for midpoints - already rounded away from zero!
      res = Cstar.w[0];	// the result is positive
      if (tmp_inexact)
	*pfpsf |= INEXACT_EXCEPTION;
    } else if (exp == 0) {
      // 1 <= q <= 10
      // res = +C (exact)
      res = C1.w[0];
    } else {	// if (exp > 0) => 1 <= exp <= 9, 1 <= q < 9, 2 <= q + exp <= 10
      // res = +C * 10^exp (exact)
      res = C1.w[0] * ten2k64[exp];
    }
  }
}

BID_RETURN (res);
}
