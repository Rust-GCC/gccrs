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
 *  BID64_to_int64_rnint
 ****************************************************************************/

#if DECIMAL_CALL_BY_REFERENCE
void
bid64_to_int64_rnint (SINT64 * pres, UINT64 * px
		      _EXC_FLAGS_PARAM _EXC_MASKS_PARAM
		      _EXC_INFO_PARAM) {
  UINT64 x = *px;
#else
SINT64
bid64_to_int64_rnint (UINT64 x
		      _EXC_FLAGS_PARAM _EXC_MASKS_PARAM
		      _EXC_INFO_PARAM) {
#endif
  SINT64 res;
  UINT64 x_sign;
  UINT64 x_exp;
  int exp;			// unbiased exponent
  // Note: C1 represents x_significand (UINT64)
  BID_UI64DOUBLE tmp1;
  unsigned int x_nr_bits;
  int q, ind, shift;
  UINT64 C1;
  UINT128 C;
  UINT64 Cstar;			// C* represents up to 16 decimal digits ~ 54 bits
  UINT128 fstar;
  UINT128 P128;

  // check for NaN or Infinity
  if ((x & MASK_NAN) == MASK_NAN || (x & MASK_INF) == MASK_INF) {
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  }
  // unpack x
  x_sign = x & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
  // if steering bits are 11 (condition will be 0), then exponent is G[0:w+1] =>
  if ((x & MASK_STEERING_BITS) == MASK_STEERING_BITS) {
    x_exp = (x & MASK_BINARY_EXPONENT2) >> 51;	// biased
    C1 = (x & MASK_BINARY_SIG2) | MASK_BINARY_OR2;
    if (C1 > 9999999999999999ull) {	// non-canonical
      x_exp = 0;
      C1 = 0;
    }
  } else {
    x_exp = (x & MASK_BINARY_EXPONENT1) >> 53;	// biased
    C1 = x & MASK_BINARY_SIG1;
  }

  // check for zeros (possibly from non-canonical values)
  if (C1 == 0x0ull) {
    // x is 0
    res = 0x00000000;
    BID_RETURN (res);
  }
  // x is not special and is not zero

  // q = nr. of decimal digits in x (1 <= q <= 54)
  //  determine first the nr. of bits in x
  if (C1 >= 0x0020000000000000ull) {	// x >= 2^53
    // split the 64-bit value in two 32-bit halves to avoid rounding errors
    if (C1 >= 0x0000000100000000ull) {	// x >= 2^32
      tmp1.d = (double) (C1 >> 32);	// exact conversion
      x_nr_bits =
	33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    } else {	// x < 2^32
      tmp1.d = (double) C1;	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// if x < 2^53
    tmp1.d = (double) C1;	// exact conversion
    x_nr_bits =
      1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1 >= nr_digits[x_nr_bits - 1].threshold_lo)
      q++;
  }
  exp = x_exp - 398;	// unbiased exponent

  if ((q + exp) > 19) {	// x >= 10^19 ~= 2^63.11... (cannot fit in SINT64)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  } else if ((q + exp) == 19) {	// x = c(0)c(1)...c(18).c(19)...c(q-1)
    // in this case 2^63.11... ~= 10^19 <= x < 10^20 ~= 2^66.43...
    // so x rounded to an integer may or may not fit in a signed 64-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 19'
    if (x_sign) {	// if n < 0 and q + exp = 19
      // if n < -2^63 - 1/2 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) > 2^63+1/2
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 > 5*(2^64+1), 1<=q<=16
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 > 0x50000000000000005, 1<=q<=16
      // <=> C * 10^(20-q) > 0x50000000000000005, 1<=q<=16
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      // Note: C1 * 10^(11-q) has 19 or 20 digits; 0x50000000000000005, has 20
      if (C.w[1] > 0x05ull || (C.w[1] == 0x05ull && C.w[0] > 0x05ull)) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    } else {	// if n > 0 and q + exp = 19
      // if n >= 2^63 - 1/2 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) >= 2^63-1/2
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 >= 5*(2^64-1), 1<=q<=16
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 >= 0x4fffffffffffffffb, 1<=q<=16
      // <=> if C * 10^(20-q) >= 0x4fffffffffffffffb, 1<=q<=16
      C.w[1] = 0x0000000000000004ull;
      C.w[0] = 0xfffffffffffffffbull;
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      if (C.w[1] > 0x04ull ||
	  (C.w[1] == 0x04ull && C.w[0] >= 0xfffffffffffffffbull)) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    }	// end else if n > 0 and q + exp = 19
  }	// end else if ((q + exp) == 19)

  // n is not too large to be converted to int64: -2^63-1/2 <= n < 2^63-1/2
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) < 0) {	// n = +/-0.0...c(0)c(1)...c(q-1)
    // return 0
    res = 0x0000000000000000ull;
    BID_RETURN (res);
  } else if ((q + exp) == 0) {	// n = +/-0.c(0)c(1)...c(q-1)
    // if 0.c(0)c(1)...c(q-1) <= 0.5 <=> c(0)c(1)...c(q-1) <= 5 * 10^(q-1)
    //   res = 0
    // else
    //   res = +/-1
    ind = q - 1;	// 0 <= ind <= 15
    if (C1 <= midpoint64[ind]) {
      res = 0x0000000000000000ull;	// return 0
    } else if (x_sign) {	// n < 0
      res = 0xffffffffffffffffull;	// return -1
    } else {	// n > 0
      res = 0x0000000000000001ull;	// return +1
    }
  } else {	// if (1 <= q + exp <= 19, 1 <= q <= 16, -15 <= exp <= 18)
    // -2^63-1/2 <= x <= -1 or 1 <= x < 2^63-1/2 so x can be rounded
    // to nearest to a 64-bit signed integer
    if (exp < 0) {	// 2 <= q <= 16, -15 <= exp <= -1, 1 <= q + exp <= 19
      ind = -exp;	// 1 <= ind <= 15; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 = C1 + 1/2 * 10^ind where the result C1 fits in 64 bits
      C1 = C1 + midpoint64[ind - 1];
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 15 
      // kx = 10^(-x) = ten2mk64[ind - 1]
      // C* = (C1 + 1/2 * 10^x) * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 54 bits
      __mul_64x64_to_128MACH (P128, C1, ten2mk64[ind - 1]);
      Cstar = P128.w[1];
      fstar.w[1] = P128.w[1] & maskhigh128[ind - 1];
      fstar.w[0] = P128.w[0];
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind].w[0], e.g.
      // if x=1, T*=ten2mk128trunc[0].w[0]=0x1999999999999999
      // if (0 < f* < 10^(-x)) then the result is a midpoint
      //   if floor(C*) is even then C* = floor(C*) - logical right
      //       shift; C* has p decimal digits, correct by Prop. 1)
      //   else if floor(C*) is odd C* = floor(C*)-1 (logical right
      //       shift; C* has p decimal digits, correct by Pr. 1)
      // else
      //   C* = floor(C*) (logical right shift; C has p decimal digits,
      //       correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-64 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 39
      Cstar = Cstar >> shift;

      // if the result was a midpoint it was rounded away from zero, so
      // it will need a correction
      // check for midpoints
      if ((fstar.w[1] == 0) && fstar.w[0] &&
	  (fstar.w[0] <= ten2mk128trunc[ind - 1].w[1])) {
	// ten2mk128trunc[ind -1].w[1] is identical to 
	// ten2mk128[ind -1].w[1]
	// the result is a midpoint; round to nearest
	if (Cstar & 0x01) {	// Cstar is odd; MP in [EVEN, ODD]
	  // if floor(C*) is odd C = floor(C*) - 1; the result >= 1
	  Cstar--;	// Cstar is now even
	}	// else MP in [ODD, EVEN]
      }
      if (x_sign)
	res = -Cstar;
      else
	res = Cstar;
    } else if (exp == 0) {
      // 1 <= q <= 16
      // res = +/-C (exact)
      if (x_sign)
	res = -C1;
      else
	res = C1;
    } else {	// if (exp > 0) => 1 <= exp <= 18, 1 <= q <= 16, 2 <= q + exp <= 20
      // (the upper limit of 20 on q + exp is due to the fact that 
      // +/-C * 10^exp is guaranteed to fit in 64 bits) 
      // res = +/-C * 10^exp (exact)
      if (x_sign)
	res = -C1 * ten2k64[exp];
      else
	res = C1 * ten2k64[exp];
    }
  }
  BID_RETURN (res);
}

/*****************************************************************************
 *  BID64_to_int64_xrnint
 ****************************************************************************/

#if DECIMAL_CALL_BY_REFERENCE
void
bid64_to_int64_xrnint (SINT64 * pres, UINT64 * px
		       _EXC_FLAGS_PARAM _EXC_MASKS_PARAM
		       _EXC_INFO_PARAM) {
  UINT64 x = *px;
#else
SINT64
bid64_to_int64_xrnint (UINT64 x
		       _EXC_FLAGS_PARAM _EXC_MASKS_PARAM
		       _EXC_INFO_PARAM) {
#endif
  SINT64 res;
  UINT64 x_sign;
  UINT64 x_exp;
  int exp;			// unbiased exponent
  // Note: C1 represents x_significand (UINT64)
  UINT64 tmp64;
  BID_UI64DOUBLE tmp1;
  unsigned int x_nr_bits;
  int q, ind, shift;
  UINT64 C1;
  UINT128 C;
  UINT64 Cstar;			// C* represents up to 16 decimal digits ~ 54 bits
  UINT128 fstar;
  UINT128 P128;

  // check for NaN or Infinity
  if ((x & MASK_NAN) == MASK_NAN || (x & MASK_INF) == MASK_INF) {
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  }
  // unpack x
  x_sign = x & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
  // if steering bits are 11 (condition will be 0), then exponent is G[0:w+1] =>
  if ((x & MASK_STEERING_BITS) == MASK_STEERING_BITS) {
    x_exp = (x & MASK_BINARY_EXPONENT2) >> 51;	// biased
    C1 = (x & MASK_BINARY_SIG2) | MASK_BINARY_OR2;
    if (C1 > 9999999999999999ull) {	// non-canonical
      x_exp = 0;
      C1 = 0;
    }
  } else {
    x_exp = (x & MASK_BINARY_EXPONENT1) >> 53;	// biased
    C1 = x & MASK_BINARY_SIG1;
  }

  // check for zeros (possibly from non-canonical values)
  if (C1 == 0x0ull) {
    // x is 0
    res = 0x00000000;
    BID_RETURN (res);
  }
  // x is not special and is not zero

  // q = nr. of decimal digits in x (1 <= q <= 54)
  //  determine first the nr. of bits in x
  if (C1 >= 0x0020000000000000ull) {	// x >= 2^53
    // split the 64-bit value in two 32-bit halves to avoid rounding errors
    if (C1 >= 0x0000000100000000ull) {	// x >= 2^32
      tmp1.d = (double) (C1 >> 32);	// exact conversion
      x_nr_bits =
	33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    } else {	// x < 2^32
      tmp1.d = (double) C1;	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// if x < 2^53
    tmp1.d = (double) C1;	// exact conversion
    x_nr_bits =
      1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1 >= nr_digits[x_nr_bits - 1].threshold_lo)
      q++;
  }
  exp = x_exp - 398;	// unbiased exponent

  if ((q + exp) > 19) {	// x >= 10^19 ~= 2^63.11... (cannot fit in SINT64)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  } else if ((q + exp) == 19) {	// x = c(0)c(1)...c(18).c(19)...c(q-1)
    // in this case 2^63.11... ~= 10^19 <= x < 10^20 ~= 2^66.43...
    // so x rounded to an integer may or may not fit in a signed 64-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 19'
    if (x_sign) {	// if n < 0 and q + exp = 19
      // if n < -2^63 - 1/2 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) > 2^63+1/2
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 > 5*(2^64+1), 1<=q<=16
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 > 0x50000000000000005, 1<=q<=16
      // <=> C * 10^(20-q) > 0x50000000000000005, 1<=q<=16
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      // Note: C1 * 10^(11-q) has 19 or 20 digits; 0x50000000000000005, has 20
      if (C.w[1] > 0x05ull || (C.w[1] == 0x05ull && C.w[0] > 0x05ull)) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    } else {	// if n > 0 and q + exp = 19
      // if n >= 2^63 - 1/2 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) >= 2^63-1/2
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 >= 5*(2^64-1), 1<=q<=16
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 >= 0x4fffffffffffffffb, 1<=q<=16
      // <=> if C * 10^(20-q) >= 0x4fffffffffffffffb, 1<=q<=16
      C.w[1] = 0x0000000000000004ull;
      C.w[0] = 0xfffffffffffffffbull;
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      if (C.w[1] > 0x04ull ||
	  (C.w[1] == 0x04ull && C.w[0] >= 0xfffffffffffffffbull)) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    }	// end else if n > 0 and q + exp = 19
  }	// end else if ((q + exp) == 19)

  // n is not too large to be converted to int64: -2^63-1/2 <= n < 2^63-1/2
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) < 0) {	// n = +/-0.0...c(0)c(1)...c(q-1)
    // set inexact flag
    *pfpsf |= INEXACT_EXCEPTION;
    // return 0
    res = 0x0000000000000000ull;
    BID_RETURN (res);
  } else if ((q + exp) == 0) {	// n = +/-0.c(0)c(1)...c(q-1)
    // if 0.c(0)c(1)...c(q-1) <= 0.5 <=> c(0)c(1)...c(q-1) <= 5 * 10^(q-1)
    //   res = 0
    // else
    //   res = +/-1
    ind = q - 1;	// 0 <= ind <= 15
    if (C1 <= midpoint64[ind]) {
      res = 0x0000000000000000ull;	// return 0
    } else if (x_sign) {	// n < 0
      res = 0xffffffffffffffffull;	// return -1
    } else {	// n > 0
      res = 0x0000000000000001ull;	// return +1
    }
    // set inexact flag
    *pfpsf |= INEXACT_EXCEPTION;
  } else {	// if (1 <= q + exp <= 19, 1 <= q <= 16, -15 <= exp <= 18)
    // -2^63-1/2 <= x <= -1 or 1 <= x < 2^63-1/2 so x can be rounded
    // to nearest to a 64-bit signed integer
    if (exp < 0) {	// 2 <= q <= 16, -15 <= exp <= -1, 1 <= q + exp <= 19
      ind = -exp;	// 1 <= ind <= 15; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 = C1 + 1/2 * 10^ind where the result C1 fits in 64 bits
      C1 = C1 + midpoint64[ind - 1];
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 15 
      // kx = 10^(-x) = ten2mk64[ind - 1]
      // C* = (C1 + 1/2 * 10^x) * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 54 bits
      __mul_64x64_to_128MACH (P128, C1, ten2mk64[ind - 1]);
      Cstar = P128.w[1];
      fstar.w[1] = P128.w[1] & maskhigh128[ind - 1];
      fstar.w[0] = P128.w[0];
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind].w[0], e.g.
      // if x=1, T*=ten2mk128trunc[0].w[0]=0x1999999999999999
      // if (0 < f* < 10^(-x)) then the result is a midpoint
      //   if floor(C*) is even then C* = floor(C*) - logical right
      //       shift; C* has p decimal digits, correct by Prop. 1)
      //   else if floor(C*) is odd C* = floor(C*)-1 (logical right
      //       shift; C* has p decimal digits, correct by Pr. 1)
      // else
      //   C* = floor(C*) (logical right shift; C has p decimal digits,
      //       correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-64 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 39
      Cstar = Cstar >> shift;
      // determine inexactness of the rounding of C*
      // if (0 < f* - 1/2 < 10^(-x)) then
      //   the result is exact
      // else // if (f* - 1/2 > T*) then
      //   the result is inexact
      if (ind - 1 <= 2) {
	if (fstar.w[0] > 0x8000000000000000ull) {
	  // f* > 1/2 and the result may be exact
	  tmp64 = fstar.w[0] - 0x8000000000000000ull;	// f* - 1/2
	  if ((tmp64 > ten2mk128trunc[ind - 1].w[1])) {
	    // ten2mk128trunc[ind -1].w[1] is identical to 
	    // ten2mk128[ind -1].w[1]
	    // set the inexact flag
	    *pfpsf |= INEXACT_EXCEPTION;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	}
      } else {	// if 3 <= ind - 1 <= 14
	if (fstar.w[1] > onehalf128[ind - 1] ||
	    (fstar.w[1] == onehalf128[ind - 1] && fstar.w[0])) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[1] - onehalf128[ind - 1];
	  if (tmp64 || fstar.w[0] > ten2mk128trunc[ind - 1].w[1]) {
	    // ten2mk128trunc[ind -1].w[1] is identical to 
	    // ten2mk128[ind -1].w[1]
	    // set the inexact flag
	    *pfpsf |= INEXACT_EXCEPTION;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	}
      }

      // if the result was a midpoint it was rounded away from zero, so
      // it will need a correction
      // check for midpoints
      if ((fstar.w[1] == 0) && fstar.w[0] &&
	  (fstar.w[0] <= ten2mk128trunc[ind - 1].w[1])) {
	// ten2mk128trunc[ind -1].w[1] is identical to 
	// ten2mk128[ind -1].w[1]
	// the result is a midpoint; round to nearest
	if (Cstar & 0x01) {	// Cstar is odd; MP in [EVEN, ODD]
	  // if floor(C*) is odd C = floor(C*) - 1; the result >= 1
	  Cstar--;	// Cstar is now even
	}	// else MP in [ODD, EVEN]
      }
      if (x_sign)
	res = -Cstar;
      else
	res = Cstar;
    } else if (exp == 0) {
      // 1 <= q <= 16
      // res = +/-C (exact)
      if (x_sign)
	res = -C1;
      else
	res = C1;
    } else {	// if (exp > 0) => 1 <= exp <= 18, 1 <= q <= 16, 2 <= q + exp <= 20
      // (the upper limit of 20 on q + exp is due to the fact that 
      // +/-C * 10^exp is guaranteed to fit in 64 bits) 
      // res = +/-C * 10^exp (exact)
      if (x_sign)
	res = -C1 * ten2k64[exp];
      else
	res = C1 * ten2k64[exp];
    }
  }
  BID_RETURN (res);
}

/*****************************************************************************
 *  BID64_to_int64_floor
 ****************************************************************************/

#if DECIMAL_CALL_BY_REFERENCE
void
bid64_to_int64_floor (SINT64 * pres, UINT64 * px
		      _EXC_FLAGS_PARAM _EXC_MASKS_PARAM
		      _EXC_INFO_PARAM) {
  UINT64 x = *px;
#else
SINT64
bid64_to_int64_floor (UINT64 x
		      _EXC_FLAGS_PARAM _EXC_MASKS_PARAM
		      _EXC_INFO_PARAM) {
#endif
  SINT64 res;
  UINT64 x_sign;
  UINT64 x_exp;
  int exp;			// unbiased exponent
  // Note: C1 represents x_significand (UINT64)
  BID_UI64DOUBLE tmp1;
  unsigned int x_nr_bits;
  int q, ind, shift;
  UINT64 C1;
  UINT128 C;
  UINT64 Cstar;			// C* represents up to 16 decimal digits ~ 54 bits
  UINT128 fstar;
  UINT128 P128;

  // check for NaN or Infinity
  if ((x & MASK_NAN) == MASK_NAN || (x & MASK_INF) == MASK_INF) {
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  }
  // unpack x
  x_sign = x & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
  // if steering bits are 11 (condition will be 0), then exponent is G[0:w+1] =>
  if ((x & MASK_STEERING_BITS) == MASK_STEERING_BITS) {
    x_exp = (x & MASK_BINARY_EXPONENT2) >> 51;	// biased
    C1 = (x & MASK_BINARY_SIG2) | MASK_BINARY_OR2;
    if (C1 > 9999999999999999ull) {	// non-canonical
      x_exp = 0;
      C1 = 0;
    }
  } else {
    x_exp = (x & MASK_BINARY_EXPONENT1) >> 53;	// biased
    C1 = x & MASK_BINARY_SIG1;
  }

  // check for zeros (possibly from non-canonical values)
  if (C1 == 0x0ull) {
    // x is 0
    res = 0x0000000000000000ull;
    BID_RETURN (res);
  }
  // x is not special and is not zero

  // q = nr. of decimal digits in x (1 <= q <= 54)
  //  determine first the nr. of bits in x
  if (C1 >= 0x0020000000000000ull) {	// x >= 2^53
    // split the 64-bit value in two 32-bit halves to avoid rounding errors
    if (C1 >= 0x0000000100000000ull) {	// x >= 2^32
      tmp1.d = (double) (C1 >> 32);	// exact conversion
      x_nr_bits =
	33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    } else {	// x < 2^32
      tmp1.d = (double) C1;	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// if x < 2^53
    tmp1.d = (double) C1;	// exact conversion
    x_nr_bits =
      1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1 >= nr_digits[x_nr_bits - 1].threshold_lo)
      q++;
  }
  exp = x_exp - 398;	// unbiased exponent

  if ((q + exp) > 19) {	// x >= 10^19 ~= 2^63.11... (cannot fit in SINT64)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  } else if ((q + exp) == 19) {	// x = c(0)c(1)...c(18).c(19)...c(q-1)
    // in this case 2^63.11... ~= 10^19 <= x < 10^20 ~= 2^66.43...
    // so x rounded to an integer may or may not fit in a signed 64-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 19'
    if (x_sign) {	// if n < 0 and q + exp = 19
      // if n < -2^63 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) > 2^63
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 >= 5*2^64, 1<=q<=16
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 > 0x50000000000000000, 1<=q<=16
      // <=> C * 10^(20-q) > 0x50000000000000000, 1<=q<=16
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      // Note: C1 * 10^(11-q) has 19 or 20 digits; 0x5000000000000000a, has 20
      if (C.w[1] > 0x05ull || (C.w[1] == 0x05ull && C.w[0] != 0)) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    } else {	// if n > 0 and q + exp = 19
      // if n >= 2^63 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) >= 2^63
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 >= 5*2^64, 1<=q<=16
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 >= 0x50000000000000000, 1<=q<=16
      // <=> if C * 10^(20-q) >= 0x50000000000000000, 1<=q<=16
      C.w[1] = 0x0000000000000005ull;
      C.w[0] = 0x0000000000000000ull;
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      if (C.w[1] >= 0x05ull) {
	// actually C.w[1] == 0x05ull && C.w[0] >= 0x0000000000000000ull) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    }	// end else if n > 0 and q + exp = 19
  }	// end else if ((q + exp) == 19)

  // n is not too large to be converted to int64: -2^63 <= n < 2^63
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) <= 0) {	// n = +/-0.0...c(0)c(1)...c(q-1)
    // return -1 or 0
    if (x_sign)
      res = 0xffffffffffffffffull;
    else
      res = 0x0000000000000000ull;
    BID_RETURN (res);
  } else {	// if (1 <= q + exp <= 19, 1 <= q <= 16, -15 <= exp <= 18)
    // -2^63 <= x <= -1 or 1 <= x < 2^63 so x can be rounded
    // to nearest to a 64-bit signed integer
    if (exp < 0) {	// 2 <= q <= 16, -15 <= exp <= -1, 1 <= q + exp <= 19
      ind = -exp;	// 1 <= ind <= 15; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 fits in 64 bits
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 15 
      // kx = 10^(-x) = ten2mk64[ind - 1]
      // C* = C1 * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 54 bits
      __mul_64x64_to_128MACH (P128, C1, ten2mk64[ind - 1]);
      Cstar = P128.w[1];
      fstar.w[1] = P128.w[1] & maskhigh128[ind - 1];
      fstar.w[0] = P128.w[0];
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind].w[0], e.g.
      // if x=1, T*=ten2mk128trunc[0].w[0]=0x1999999999999999
      // C* = floor(C*) (logical right shift; C has p decimal digits,
      //     correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-64 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 39
      Cstar = Cstar >> shift;
      // determine inexactness of the rounding of C*
      // if (0 < f* < 10^(-x)) then
      //   the result is exact
      // else // if (f* > T*) then
      //   the result is inexact
      if (ind - 1 <= 2) {	// fstar.w[1] is 0
	if (fstar.w[0] > ten2mk128trunc[ind - 1].w[1]) {
	  // ten2mk128trunc[ind -1].w[1] is identical to
	  // ten2mk128[ind -1].w[1]
	  if (x_sign) {	// negative and inexact
	    Cstar++;
	  }
	}	// else the result is exact
      } else {	// if 3 <= ind - 1 <= 14
	if (fstar.w[1] || fstar.w[0] > ten2mk128trunc[ind - 1].w[1]) {
	  // ten2mk128trunc[ind -1].w[1] is identical to
	  // ten2mk128[ind -1].w[1]
	  if (x_sign) {	// negative and inexact
	    Cstar++;
	  }
	}	// else the result is exact
      }

      if (x_sign)
	res = -Cstar;
      else
	res = Cstar;
    } else if (exp == 0) {
      // 1 <= q <= 16
      // res = +/-C (exact)
      if (x_sign)
	res = -C1;
      else
	res = C1;
    } else {	// if (exp > 0) => 1 <= exp <= 18, 1 <= q <= 16, 2 <= q + exp <= 20
      // (the upper limit of 20 on q + exp is due to the fact that 
      // +/-C * 10^exp is guaranteed to fit in 64 bits) 
      // res = +/-C * 10^exp (exact)
      if (x_sign)
	res = -C1 * ten2k64[exp];
      else
	res = C1 * ten2k64[exp];
    }
  }
  BID_RETURN (res);
}

/*****************************************************************************
 *  BID64_to_int64_xfloor
 ****************************************************************************/

#if DECIMAL_CALL_BY_REFERENCE
void
bid64_to_int64_xfloor (SINT64 * pres, UINT64 * px
		       _EXC_FLAGS_PARAM _EXC_MASKS_PARAM
		       _EXC_INFO_PARAM) {
  UINT64 x = *px;
#else
SINT64
bid64_to_int64_xfloor (UINT64 x
		       _EXC_FLAGS_PARAM _EXC_MASKS_PARAM
		       _EXC_INFO_PARAM) {
#endif
  SINT64 res;
  UINT64 x_sign;
  UINT64 x_exp;
  int exp;			// unbiased exponent
  // Note: C1 represents x_significand (UINT64)
  BID_UI64DOUBLE tmp1;
  unsigned int x_nr_bits;
  int q, ind, shift;
  UINT64 C1;
  UINT128 C;
  UINT64 Cstar;			// C* represents up to 16 decimal digits ~ 54 bits
  UINT128 fstar;
  UINT128 P128;

  // check for NaN or Infinity
  if ((x & MASK_NAN) == MASK_NAN || (x & MASK_INF) == MASK_INF) {
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  }
  // unpack x
  x_sign = x & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
  // if steering bits are 11 (condition will be 0), then exponent is G[0:w+1] =>
  if ((x & MASK_STEERING_BITS) == MASK_STEERING_BITS) {
    x_exp = (x & MASK_BINARY_EXPONENT2) >> 51;	// biased
    C1 = (x & MASK_BINARY_SIG2) | MASK_BINARY_OR2;
    if (C1 > 9999999999999999ull) {	// non-canonical
      x_exp = 0;
      C1 = 0;
    }
  } else {
    x_exp = (x & MASK_BINARY_EXPONENT1) >> 53;	// biased
    C1 = x & MASK_BINARY_SIG1;
  }

  // check for zeros (possibly from non-canonical values)
  if (C1 == 0x0ull) {
    // x is 0
    res = 0x0000000000000000ull;
    BID_RETURN (res);
  }
  // x is not special and is not zero

  // q = nr. of decimal digits in x (1 <= q <= 54)
  //  determine first the nr. of bits in x
  if (C1 >= 0x0020000000000000ull) {	// x >= 2^53
    // split the 64-bit value in two 32-bit halves to avoid rounding errors
    if (C1 >= 0x0000000100000000ull) {	// x >= 2^32
      tmp1.d = (double) (C1 >> 32);	// exact conversion
      x_nr_bits =
	33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    } else {	// x < 2^32
      tmp1.d = (double) C1;	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// if x < 2^53
    tmp1.d = (double) C1;	// exact conversion
    x_nr_bits =
      1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1 >= nr_digits[x_nr_bits - 1].threshold_lo)
      q++;
  }
  exp = x_exp - 398;	// unbiased exponent

  if ((q + exp) > 19) {	// x >= 10^19 ~= 2^63.11... (cannot fit in SINT64)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  } else if ((q + exp) == 19) {	// x = c(0)c(1)...c(18).c(19)...c(q-1)
    // in this case 2^63.11... ~= 10^19 <= x < 10^20 ~= 2^66.43...
    // so x rounded to an integer may or may not fit in a signed 64-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 19'
    if (x_sign) {	// if n < 0 and q + exp = 19
      // if n < -2^63 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) > 2^63
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 >= 5*2^64, 1<=q<=16
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 > 0x50000000000000000, 1<=q<=16
      // <=> C * 10^(20-q) > 0x50000000000000000, 1<=q<=16
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      // Note: C1 * 10^(11-q) has 19 or 20 digits; 0x5000000000000000a, has 20
      if (C.w[1] > 0x05ull || (C.w[1] == 0x05ull && C.w[0] != 0)) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    } else {	// if n > 0 and q + exp = 19
      // if n >= 2^63 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) >= 2^63
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 >= 5*2^64, 1<=q<=16
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 >= 0x50000000000000000, 1<=q<=16
      // <=> if C * 10^(20-q) >= 0x50000000000000000, 1<=q<=16
      C.w[1] = 0x0000000000000005ull;
      C.w[0] = 0x0000000000000000ull;
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      if (C.w[1] >= 0x05ull) {
	// actually C.w[1] == 0x05ull && C.w[0] >= 0x0000000000000000ull) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    }	// end else if n > 0 and q + exp = 19
  }	// end else if ((q + exp) == 19)

  // n is not too large to be converted to int64: -2^63 <= n < 2^63
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) <= 0) {	// n = +/-0.0...c(0)c(1)...c(q-1)
    // set inexact flag
    *pfpsf |= INEXACT_EXCEPTION;
    // return -1 or 0
    if (x_sign)
      res = 0xffffffffffffffffull;
    else
      res = 0x0000000000000000ull;
    BID_RETURN (res);
  } else {	// if (1 <= q + exp <= 19, 1 <= q <= 16, -15 <= exp <= 18)
    // -2^63 <= x <= -1 or 1 <= x < 2^63 so x can be rounded
    // to nearest to a 64-bit signed integer
    if (exp < 0) {	// 2 <= q <= 16, -15 <= exp <= -1, 1 <= q + exp <= 19
      ind = -exp;	// 1 <= ind <= 15; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 fits in 64 bits
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 15 
      // kx = 10^(-x) = ten2mk64[ind - 1]
      // C* = C1 * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 54 bits
      __mul_64x64_to_128MACH (P128, C1, ten2mk64[ind - 1]);
      Cstar = P128.w[1];
      fstar.w[1] = P128.w[1] & maskhigh128[ind - 1];
      fstar.w[0] = P128.w[0];
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind].w[0], e.g.
      // if x=1, T*=ten2mk128trunc[0].w[0]=0x1999999999999999
      // C* = floor(C*) (logical right shift; C has p decimal digits,
      //     correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-64 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 39
      Cstar = Cstar >> shift;
      // determine inexactness of the rounding of C*
      // if (0 < f* < 10^(-x)) then
      //   the result is exact
      // else // if (f* > T*) then
      //   the result is inexact
      if (ind - 1 <= 2) {	// fstar.w[1] is 0
	if (fstar.w[0] > ten2mk128trunc[ind - 1].w[1]) {
	  // ten2mk128trunc[ind -1].w[1] is identical to
	  // ten2mk128[ind -1].w[1]
	  if (x_sign) {	// negative and inexact
	    Cstar++;
	  }
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	}	// else the result is exact
      } else {	// if 3 <= ind - 1 <= 14
	if (fstar.w[1] || fstar.w[0] > ten2mk128trunc[ind - 1].w[1]) {
	  // ten2mk128trunc[ind -1].w[1] is identical to
	  // ten2mk128[ind -1].w[1]
	  if (x_sign) {	// negative and inexact
	    Cstar++;
	  }
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	}	// else the result is exact
      }

      if (x_sign)
	res = -Cstar;
      else
	res = Cstar;
    } else if (exp == 0) {
      // 1 <= q <= 16
      // res = +/-C (exact)
      if (x_sign)
	res = -C1;
      else
	res = C1;
    } else {	// if (exp > 0) => 1 <= exp <= 18, 1 <= q <= 16, 2 <= q + exp <= 20
      // (the upper limit of 20 on q + exp is due to the fact that 
      // +/-C * 10^exp is guaranteed to fit in 64 bits) 
      // res = +/-C * 10^exp (exact)
      if (x_sign)
	res = -C1 * ten2k64[exp];
      else
	res = C1 * ten2k64[exp];
    }
  }
  BID_RETURN (res);
}

/*****************************************************************************
 *  BID64_to_int64_ceil
 ****************************************************************************/

#if DECIMAL_CALL_BY_REFERENCE
void
bid64_to_int64_ceil (SINT64 * pres, UINT64 * px
		     _EXC_FLAGS_PARAM _EXC_MASKS_PARAM _EXC_INFO_PARAM) 
{
  UINT64 x = *px;
#else
SINT64
bid64_to_int64_ceil (UINT64 x
		     _EXC_FLAGS_PARAM _EXC_MASKS_PARAM _EXC_INFO_PARAM) 
{
#endif
  SINT64 res;
  UINT64 x_sign;
  UINT64 x_exp;
  int exp;			// unbiased exponent
  // Note: C1 represents x_significand (UINT64)
  BID_UI64DOUBLE tmp1;
  unsigned int x_nr_bits;
  int q, ind, shift;
  UINT64 C1;
  UINT128 C;
  UINT64 Cstar;			// C* represents up to 16 decimal digits ~ 54 bits
  UINT128 fstar;
  UINT128 P128;

  // check for NaN or Infinity
  if ((x & MASK_NAN) == MASK_NAN || (x & MASK_INF) == MASK_INF) {
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  }
  // unpack x
  x_sign = x & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
  // if steering bits are 11 (condition will be 0), then exponent is G[0:w+1] =>
  if ((x & MASK_STEERING_BITS) == MASK_STEERING_BITS) {
    x_exp = (x & MASK_BINARY_EXPONENT2) >> 51;	// biased
    C1 = (x & MASK_BINARY_SIG2) | MASK_BINARY_OR2;
    if (C1 > 9999999999999999ull) {	// non-canonical
      x_exp = 0;
      C1 = 0;
    }
  } else {
    x_exp = (x & MASK_BINARY_EXPONENT1) >> 53;	// biased
    C1 = x & MASK_BINARY_SIG1;
  }

  // check for zeros (possibly from non-canonical values)
  if (C1 == 0x0ull) {
    // x is 0
    res = 0x00000000;
    BID_RETURN (res);
  }
  // x is not special and is not zero

  // q = nr. of decimal digits in x (1 <= q <= 54)
  //  determine first the nr. of bits in x
  if (C1 >= 0x0020000000000000ull) {	// x >= 2^53
    // split the 64-bit value in two 32-bit halves to avoid rounding errors
    if (C1 >= 0x0000000100000000ull) {	// x >= 2^32
      tmp1.d = (double) (C1 >> 32);	// exact conversion
      x_nr_bits =
	33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    } else {	// x < 2^32
      tmp1.d = (double) C1;	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// if x < 2^53
    tmp1.d = (double) C1;	// exact conversion
    x_nr_bits =
      1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1 >= nr_digits[x_nr_bits - 1].threshold_lo)
      q++;
  }
  exp = x_exp - 398;	// unbiased exponent

  if ((q + exp) > 19) {	// x >= 10^19 ~= 2^63.11... (cannot fit in SINT64)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  } else if ((q + exp) == 19) {	// x = c(0)c(1)...c(18).c(19)...c(q-1)
    // in this case 2^63.11... ~= 10^19 <= x < 10^20 ~= 2^66.43...
    // so x rounded to an integer may or may not fit in a signed 64-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 19'
    if (x_sign) {	// if n < 0 and q + exp = 19
      // if n <= -2^63 - 1 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) >= 2^63+1
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 >= 5*(2^64+2), 1<=q<=16
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 >= 0x5000000000000000a, 1<=q<=16
      // <=> C * 10^(20-q) >= 0x5000000000000000a, 1<=q<=16
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      // Note: C1 * 10^(11-q) has 19 or 20 digits; 0x5000000000000000a, has 20
      if (C.w[1] > 0x05ull || (C.w[1] == 0x05ull && C.w[0] >= 0x0aull)) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    } else {	// if n > 0 and q + exp = 19
      // if n > 2^63 - 1 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) > 2^63 - 1
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 > 5*(2^64-2), 1<=q<=16
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 > 0x4fffffffffffffff6, 1<=q<=16
      // <=> if C * 10^(20-q) > 0x4fffffffffffffff6, 1<=q<=16
      C.w[1] = 0x0000000000000004ull;
      C.w[0] = 0xfffffffffffffff6ull;
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      if (C.w[1] > 0x04ull ||
	  (C.w[1] == 0x04ull && C.w[0] > 0xfffffffffffffff6ull)) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    }	// end else if n > 0 and q + exp = 19
  }	// end else if ((q + exp) == 19)

  // n is not too large to be converted to int64: -2^63-1 < n < 2^63
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) <= 0) {	// n = +/-0.0...c(0)c(1)...c(q-1)
    // return 0 or 1
    if (x_sign)
      res = 0x00000000;
    else
      res = 0x00000001;
    BID_RETURN (res);
  } else {	// if (1 <= q + exp <= 19, 1 <= q <= 16, -15 <= exp <= 18)
    // -2^63-1 < x <= -1 or 1 <= x <= 2^63 - 1 so x can be rounded
    // to nearest to a 64-bit signed integer
    if (exp < 0) {	// 2 <= q <= 16, -15 <= exp <= -1, 1 <= q + exp <= 19
      ind = -exp;	// 1 <= ind <= 15; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 fits in 64 bits
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 15 
      // kx = 10^(-x) = ten2mk64[ind - 1]
      // C* = C1 * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 54 bits
      __mul_64x64_to_128MACH (P128, C1, ten2mk64[ind - 1]);
      Cstar = P128.w[1];
      fstar.w[1] = P128.w[1] & maskhigh128[ind - 1];
      fstar.w[0] = P128.w[0];
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind].w[0], e.g.
      // if x=1, T*=ten2mk128trunc[0].w[0]=0x1999999999999999
      // C* = floor(C*) (logical right shift; C has p decimal digits,
      //     correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-64 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 39
      Cstar = Cstar >> shift;
      // determine inexactness of the rounding of C*
      // if (0 < f* < 10^(-x)) then
      //   the result is exact
      // else // if (f* > T*) then
      //   the result is inexact
      if (ind - 1 <= 2) {	// fstar.w[1] is 0
	if (fstar.w[0] > ten2mk128trunc[ind - 1].w[1]) {
	  // ten2mk128trunc[ind -1].w[1] is identical to
	  // ten2mk128[ind -1].w[1]
	  if (!x_sign) {	// positive and inexact
	    Cstar++;
	  }
	}	// else the result is exact
      } else {	// if 3 <= ind - 1 <= 14
	if (fstar.w[1] || fstar.w[0] > ten2mk128trunc[ind - 1].w[1]) {
	  // ten2mk128trunc[ind -1].w[1] is identical to
	  // ten2mk128[ind -1].w[1]
	  if (!x_sign) {	// positive and inexact
	    Cstar++;
	  }
	}	// else the result is exact
      }

      if (x_sign)
	res = -Cstar;
      else
	res = Cstar;
    } else if (exp == 0) {
      // 1 <= q <= 16
      // res = +/-C (exact)
      if (x_sign)
	res = -C1;
      else
	res = C1;
    } else {	// if (exp > 0) => 1 <= exp <= 18, 1 <= q <= 16, 2 <= q + exp <= 20
      // (the upper limit of 20 on q + exp is due to the fact that 
      // +/-C * 10^exp is guaranteed to fit in 64 bits) 
      // res = +/-C * 10^exp (exact)
      if (x_sign)
	res = -C1 * ten2k64[exp];
      else
	res = C1 * ten2k64[exp];
    }
  }
  BID_RETURN (res);
}

/*****************************************************************************
 *  BID64_to_int64_xceil
 ****************************************************************************/

#if DECIMAL_CALL_BY_REFERENCE
void
bid64_to_int64_xceil (SINT64 * pres, UINT64 * px
		      _EXC_FLAGS_PARAM _EXC_MASKS_PARAM
		      _EXC_INFO_PARAM) {
  UINT64 x = *px;
#else
SINT64
bid64_to_int64_xceil (UINT64 x
		      _EXC_FLAGS_PARAM _EXC_MASKS_PARAM
		      _EXC_INFO_PARAM) {
#endif
  SINT64 res;
  UINT64 x_sign;
  UINT64 x_exp;
  int exp;			// unbiased exponent
  // Note: C1 represents x_significand (UINT64)
  BID_UI64DOUBLE tmp1;
  unsigned int x_nr_bits;
  int q, ind, shift;
  UINT64 C1;
  UINT128 C;
  UINT64 Cstar;			// C* represents up to 16 decimal digits ~ 54 bits
  UINT128 fstar;
  UINT128 P128;

  // check for NaN or Infinity
  if ((x & MASK_NAN) == MASK_NAN || (x & MASK_INF) == MASK_INF) {
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  }
  // unpack x
  x_sign = x & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
  // if steering bits are 11 (condition will be 0), then exponent is G[0:w+1] =>
  if ((x & MASK_STEERING_BITS) == MASK_STEERING_BITS) {
    x_exp = (x & MASK_BINARY_EXPONENT2) >> 51;	// biased
    C1 = (x & MASK_BINARY_SIG2) | MASK_BINARY_OR2;
    if (C1 > 9999999999999999ull) {	// non-canonical
      x_exp = 0;
      C1 = 0;
    }
  } else {
    x_exp = (x & MASK_BINARY_EXPONENT1) >> 53;	// biased
    C1 = x & MASK_BINARY_SIG1;
  }

  // check for zeros (possibly from non-canonical values)
  if (C1 == 0x0ull) {
    // x is 0
    res = 0x00000000;
    BID_RETURN (res);
  }
  // x is not special and is not zero

  // q = nr. of decimal digits in x (1 <= q <= 54)
  //  determine first the nr. of bits in x
  if (C1 >= 0x0020000000000000ull) {	// x >= 2^53
    // split the 64-bit value in two 32-bit halves to avoid rounding errors
    if (C1 >= 0x0000000100000000ull) {	// x >= 2^32
      tmp1.d = (double) (C1 >> 32);	// exact conversion
      x_nr_bits =
	33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    } else {	// x < 2^32
      tmp1.d = (double) C1;	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// if x < 2^53
    tmp1.d = (double) C1;	// exact conversion
    x_nr_bits =
      1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1 >= nr_digits[x_nr_bits - 1].threshold_lo)
      q++;
  }
  exp = x_exp - 398;	// unbiased exponent

  if ((q + exp) > 19) {	// x >= 10^19 ~= 2^63.11... (cannot fit in SINT64)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  } else if ((q + exp) == 19) {	// x = c(0)c(1)...c(18).c(19)...c(q-1)
    // in this case 2^63.11... ~= 10^19 <= x < 10^20 ~= 2^66.43...
    // so x rounded to an integer may or may not fit in a signed 64-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 19'
    if (x_sign) {	// if n < 0 and q + exp = 19
      // if n <= -2^63 - 1 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) >= 2^63+1
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 >= 5*(2^64+2), 1<=q<=16
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 >= 0x5000000000000000a, 1<=q<=16
      // <=> C * 10^(20-q) >= 0x5000000000000000a, 1<=q<=16
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      // Note: C1 * 10^(11-q) has 19 or 20 digits; 0x5000000000000000a, has 20
      if (C.w[1] > 0x05ull || (C.w[1] == 0x05ull && C.w[0] >= 0x0aull)) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    } else {	// if n > 0 and q + exp = 19
      // if n > 2^63 - 1 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) > 2^63 - 1
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 > 5*(2^64-2), 1<=q<=16
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 > 0x4fffffffffffffff6, 1<=q<=16
      // <=> if C * 10^(20-q) > 0x4fffffffffffffff6, 1<=q<=16
      C.w[1] = 0x0000000000000004ull;
      C.w[0] = 0xfffffffffffffff6ull;
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      if (C.w[1] > 0x04ull ||
	  (C.w[1] == 0x04ull && C.w[0] > 0xfffffffffffffff6ull)) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    }	// end else if n > 0 and q + exp = 19
  }	// end else if ((q + exp) == 19)

  // n is not too large to be converted to int64: -2^63-1 < n < 2^63
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) <= 0) {	// n = +/-0.0...c(0)c(1)...c(q-1)
    // set inexact flag
    *pfpsf |= INEXACT_EXCEPTION;
    // return 0 or 1
    if (x_sign)
      res = 0x00000000;
    else
      res = 0x00000001;
    BID_RETURN (res);
  } else {	// if (1 <= q + exp <= 19, 1 <= q <= 16, -15 <= exp <= 18)
    // -2^63-1 < x <= -1 or 1 <= x <= 2^63 - 1 so x can be rounded
    // to nearest to a 64-bit signed integer
    if (exp < 0) {	// 2 <= q <= 16, -15 <= exp <= -1, 1 <= q + exp <= 19
      ind = -exp;	// 1 <= ind <= 15; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 fits in 64 bits
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 15 
      // kx = 10^(-x) = ten2mk64[ind - 1]
      // C* = C1 * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 54 bits
      __mul_64x64_to_128MACH (P128, C1, ten2mk64[ind - 1]);
      Cstar = P128.w[1];
      fstar.w[1] = P128.w[1] & maskhigh128[ind - 1];
      fstar.w[0] = P128.w[0];
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind].w[0], e.g.
      // if x=1, T*=ten2mk128trunc[0].w[0]=0x1999999999999999
      // C* = floor(C*) (logical right shift; C has p decimal digits,
      //     correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-64 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 39
      Cstar = Cstar >> shift;
      // determine inexactness of the rounding of C*
      // if (0 < f* < 10^(-x)) then
      //   the result is exact
      // else // if (f* > T*) then
      //   the result is inexact
      if (ind - 1 <= 2) {	// fstar.w[1] is 0
	if (fstar.w[0] > ten2mk128trunc[ind - 1].w[1]) {
	  // ten2mk128trunc[ind -1].w[1] is identical to
	  // ten2mk128[ind -1].w[1]
	  if (!x_sign) {	// positive and inexact
	    Cstar++;
	  }
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	}	// else the result is exact
      } else {	// if 3 <= ind - 1 <= 14
	if (fstar.w[1] || fstar.w[0] > ten2mk128trunc[ind - 1].w[1]) {
	  // ten2mk128trunc[ind -1].w[1] is identical to
	  // ten2mk128[ind -1].w[1]
	  if (!x_sign) {	// positive and inexact
	    Cstar++;
	  }
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	}	// else the result is exact
      }

      if (x_sign)
	res = -Cstar;
      else
	res = Cstar;
    } else if (exp == 0) {
      // 1 <= q <= 16
      // res = +/-C (exact)
      if (x_sign)
	res = -C1;
      else
	res = C1;
    } else {	// if (exp > 0) => 1 <= exp <= 18, 1 <= q <= 16, 2 <= q + exp <= 20
      // (the upper limit of 20 on q + exp is due to the fact that 
      // +/-C * 10^exp is guaranteed to fit in 64 bits) 
      // res = +/-C * 10^exp (exact)
      if (x_sign)
	res = -C1 * ten2k64[exp];
      else
	res = C1 * ten2k64[exp];
    }
  }
  BID_RETURN (res);
}

/*****************************************************************************
 *  BID64_to_int64_int
 ****************************************************************************/

#if DECIMAL_CALL_BY_REFERENCE
void
bid64_to_int64_int (SINT64 * pres, UINT64 * px
		    _EXC_FLAGS_PARAM _EXC_MASKS_PARAM _EXC_INFO_PARAM) {
  UINT64 x = *px;
#else
SINT64
bid64_to_int64_int (UINT64 x
		    _EXC_FLAGS_PARAM _EXC_MASKS_PARAM _EXC_INFO_PARAM) {
#endif
  SINT64 res;
  UINT64 x_sign;
  UINT64 x_exp;
  int exp;			// unbiased exponent
  // Note: C1 represents x_significand (UINT64)
  BID_UI64DOUBLE tmp1;
  unsigned int x_nr_bits;
  int q, ind, shift;
  UINT64 C1;
  UINT128 C;
  UINT64 Cstar;			// C* represents up to 16 decimal digits ~ 54 bits
  UINT128 P128;

  // check for NaN or Infinity
  if ((x & MASK_NAN) == MASK_NAN || (x & MASK_INF) == MASK_INF) {
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  }
  // unpack x
  x_sign = x & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
  // if steering bits are 11 (condition will be 0), then exponent is G[0:w+1] =>
  if ((x & MASK_STEERING_BITS) == MASK_STEERING_BITS) {
    x_exp = (x & MASK_BINARY_EXPONENT2) >> 51;	// biased
    C1 = (x & MASK_BINARY_SIG2) | MASK_BINARY_OR2;
    if (C1 > 9999999999999999ull) {	// non-canonical
      x_exp = 0;
      C1 = 0;
    }
  } else {
    x_exp = (x & MASK_BINARY_EXPONENT1) >> 53;	// biased
    C1 = x & MASK_BINARY_SIG1;
  }

  // check for zeros (possibly from non-canonical values)
  if (C1 == 0x0ull) {
    // x is 0
    res = 0x00000000;
    BID_RETURN (res);
  }
  // x is not special and is not zero

  // q = nr. of decimal digits in x (1 <= q <= 54)
  //  determine first the nr. of bits in x
  if (C1 >= 0x0020000000000000ull) {	// x >= 2^53
    // split the 64-bit value in two 32-bit halves to avoid rounding errors
    if (C1 >= 0x0000000100000000ull) {	// x >= 2^32
      tmp1.d = (double) (C1 >> 32);	// exact conversion
      x_nr_bits =
	33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    } else {	// x < 2^32
      tmp1.d = (double) C1;	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// if x < 2^53
    tmp1.d = (double) C1;	// exact conversion
    x_nr_bits =
      1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1 >= nr_digits[x_nr_bits - 1].threshold_lo)
      q++;
  }
  exp = x_exp - 398;	// unbiased exponent

  if ((q + exp) > 19) {	// x >= 10^19 ~= 2^63.11... (cannot fit in SINT64)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  } else if ((q + exp) == 19) {	// x = c(0)c(1)...c(18).c(19)...c(q-1)
    // in this case 2^63.11... ~= 10^19 <= x < 10^20 ~= 2^66.43...
    // so x rounded to an integer may or may not fit in a signed 64-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 19'
    if (x_sign) {	// if n < 0 and q + exp = 19
      // if n <= -2^63 - 1 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) >= 2^63+1
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 >= 5*(2^64+2), 1<=q<=16
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 >= 0x5000000000000000a, 1<=q<=16
      // <=> C * 10^(20-q) >= 0x5000000000000000a, 1<=q<=16
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      // Note: C1 * 10^(11-q) has 19 or 20 digits; 0x5000000000000000a, has 20
      if (C.w[1] > 0x05ull || (C.w[1] == 0x05ull && C.w[0] >= 0x0aull)) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    } else {	// if n > 0 and q + exp = 19
      // if n >= 2^63 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) >= 2^63
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 >= 5*2^64, 1<=q<=16
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 >= 0x50000000000000000, 1<=q<=16
      // <=> if C * 10^(20-q) >= 0x50000000000000000, 1<=q<=16
      C.w[1] = 0x0000000000000005ull;
      C.w[0] = 0x0000000000000000ull;
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      if (C.w[1] >= 0x05ull) {
	// actually C.w[1] == 0x05ull && C.w[0] >= 0x0000000000000000ull) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    }	// end else if n > 0 and q + exp = 19
  }	// end else if ((q + exp) == 19)

  // n is not too large to be converted to int64: -2^63-1 < n < 2^63
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) <= 0) {	// n = +/-0.0...c(0)c(1)...c(q-1)
    // return 0
    res = 0x0000000000000000ull;
    BID_RETURN (res);
  } else {	// if (1 <= q + exp <= 19, 1 <= q <= 16, -15 <= exp <= 18)
    // -2^63-1 < x <= -1 or 1 <= x < 2^63 so x can be rounded
    // to nearest to a 64-bit signed integer
    if (exp < 0) {	// 2 <= q <= 16, -15 <= exp <= -1, 1 <= q + exp <= 19
      ind = -exp;	// 1 <= ind <= 15; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 fits in 64 bits
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 15 
      // kx = 10^(-x) = ten2mk64[ind - 1]
      // C* = C1 * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 54 bits
      __mul_64x64_to_128MACH (P128, C1, ten2mk64[ind - 1]);
      Cstar = P128.w[1];
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind].w[0], e.g.
      // if x=1, T*=ten2mk128trunc[0].w[0]=0x1999999999999999
      // C* = floor(C*) (logical right shift; C has p decimal digits,
      //     correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-64 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 39
      Cstar = Cstar >> shift;

      if (x_sign)
	res = -Cstar;
      else
	res = Cstar;
    } else if (exp == 0) {
      // 1 <= q <= 16
      // res = +/-C (exact)
      if (x_sign)
	res = -C1;
      else
	res = C1;
    } else {	// if (exp > 0) => 1 <= exp <= 18, 1 <= q <= 16, 2 <= q + exp <= 20
      // (the upper limit of 20 on q + exp is due to the fact that 
      // +/-C * 10^exp is guaranteed to fit in 64 bits) 
      // res = +/-C * 10^exp (exact)
      if (x_sign)
	res = -C1 * ten2k64[exp];
      else
	res = C1 * ten2k64[exp];
    }
  }
  BID_RETURN (res);
}

/*****************************************************************************
 *  BID64_to_int64_xint
 ****************************************************************************/

#if DECIMAL_CALL_BY_REFERENCE
void
bid64_to_int64_xint (SINT64 * pres, UINT64 * px
		     _EXC_FLAGS_PARAM _EXC_MASKS_PARAM _EXC_INFO_PARAM) 
{
  UINT64 x = *px;
#else
SINT64
bid64_to_int64_xint (UINT64 x
		     _EXC_FLAGS_PARAM _EXC_MASKS_PARAM _EXC_INFO_PARAM) 
{
#endif
  SINT64 res;
  UINT64 x_sign;
  UINT64 x_exp;
  int exp;			// unbiased exponent
  // Note: C1 represents x_significand (UINT64)
  BID_UI64DOUBLE tmp1;
  unsigned int x_nr_bits;
  int q, ind, shift;
  UINT64 C1;
  UINT128 C;
  UINT64 Cstar;			// C* represents up to 16 decimal digits ~ 54 bits
  UINT128 fstar;
  UINT128 P128;

  // check for NaN or Infinity
  if ((x & MASK_NAN) == MASK_NAN || (x & MASK_INF) == MASK_INF) {
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  }
  // unpack x
  x_sign = x & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
  // if steering bits are 11 (condition will be 0), then exponent is G[0:w+1] =>
  if ((x & MASK_STEERING_BITS) == MASK_STEERING_BITS) {
    x_exp = (x & MASK_BINARY_EXPONENT2) >> 51;	// biased
    C1 = (x & MASK_BINARY_SIG2) | MASK_BINARY_OR2;
    if (C1 > 9999999999999999ull) {	// non-canonical
      x_exp = 0;
      C1 = 0;
    }
  } else {
    x_exp = (x & MASK_BINARY_EXPONENT1) >> 53;	// biased
    C1 = x & MASK_BINARY_SIG1;
  }

  // check for zeros (possibly from non-canonical values)
  if (C1 == 0x0ull) {
    // x is 0
    res = 0x00000000;
    BID_RETURN (res);
  }
  // x is not special and is not zero

  // q = nr. of decimal digits in x (1 <= q <= 54)
  //  determine first the nr. of bits in x
  if (C1 >= 0x0020000000000000ull) {	// x >= 2^53
    // split the 64-bit value in two 32-bit halves to avoid rounding errors
    if (C1 >= 0x0000000100000000ull) {	// x >= 2^32
      tmp1.d = (double) (C1 >> 32);	// exact conversion
      x_nr_bits =
	33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    } else {	// x < 2^32
      tmp1.d = (double) C1;	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// if x < 2^53
    tmp1.d = (double) C1;	// exact conversion
    x_nr_bits =
      1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1 >= nr_digits[x_nr_bits - 1].threshold_lo)
      q++;
  }
  exp = x_exp - 398;	// unbiased exponent

  if ((q + exp) > 19) {	// x >= 10^19 ~= 2^63.11... (cannot fit in SINT64)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  } else if ((q + exp) == 19) {	// x = c(0)c(1)...c(18).c(19)...c(q-1)
    // in this case 2^63.11... ~= 10^19 <= x < 10^20 ~= 2^66.43...
    // so x rounded to an integer may or may not fit in a signed 64-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 19'
    if (x_sign) {	// if n < 0 and q + exp = 19
      // if n <= -2^63 - 1 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) >= 2^63+1
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 >= 5*(2^64+2), 1<=q<=16
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 >= 0x5000000000000000a, 1<=q<=16
      // <=> C * 10^(20-q) >= 0x5000000000000000a, 1<=q<=16
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      // Note: C1 * 10^(11-q) has 19 or 20 digits; 0x5000000000000000a, has 20
      if (C.w[1] > 0x05ull || (C.w[1] == 0x05ull && C.w[0] >= 0x0aull)) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    } else {	// if n > 0 and q + exp = 19
      // if n >= 2^63 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) >= 2^63
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 >= 5*2^64, 1<=q<=16
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 >= 0x50000000000000000, 1<=q<=16
      // <=> if C * 10^(20-q) >= 0x50000000000000000, 1<=q<=16
      C.w[1] = 0x0000000000000005ull;
      C.w[0] = 0x0000000000000000ull;
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      if (C.w[1] >= 0x05ull) {
	// actually C.w[1] == 0x05ull && C.w[0] >= 0x0000000000000000ull) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    }	// end else if n > 0 and q + exp = 19
  }	// end else if ((q + exp) == 19)

  // n is not too large to be converted to int64: -2^63-1 < n < 2^63
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) <= 0) {	// n = +/-0.0...c(0)c(1)...c(q-1)
    // set inexact flag
    *pfpsf |= INEXACT_EXCEPTION;
    // return 0
    res = 0x0000000000000000ull;
    BID_RETURN (res);
  } else {	// if (1 <= q + exp <= 19, 1 <= q <= 16, -15 <= exp <= 18)
    // -2^63-1 < x <= -1 or 1 <= x < 2^63 so x can be rounded
    // to nearest to a 64-bit signed integer
    if (exp < 0) {	// 2 <= q <= 16, -15 <= exp <= -1, 1 <= q + exp <= 19
      ind = -exp;	// 1 <= ind <= 15; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 fits in 64 bits
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 15 
      // kx = 10^(-x) = ten2mk64[ind - 1]
      // C* = C1 * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 54 bits
      __mul_64x64_to_128MACH (P128, C1, ten2mk64[ind - 1]);
      Cstar = P128.w[1];
      fstar.w[1] = P128.w[1] & maskhigh128[ind - 1];
      fstar.w[0] = P128.w[0];
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind].w[0], e.g.
      // if x=1, T*=ten2mk128trunc[0].w[0]=0x1999999999999999
      // C* = floor(C*) (logical right shift; C has p decimal digits,
      //     correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-64 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 39
      Cstar = Cstar >> shift;
      // determine inexactness of the rounding of C*
      // if (0 < f* < 10^(-x)) then
      //   the result is exact
      // else // if (f* > T*) then
      //   the result is inexact
      if (ind - 1 <= 2) {	// fstar.w[1] is 0
	if (fstar.w[0] > ten2mk128trunc[ind - 1].w[1]) {
	  // ten2mk128trunc[ind -1].w[1] is identical to
	  // ten2mk128[ind -1].w[1]
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	}	// else the result is exact
      } else {	// if 3 <= ind - 1 <= 14
	if (fstar.w[1] || fstar.w[0] > ten2mk128trunc[ind - 1].w[1]) {
	  // ten2mk128trunc[ind -1].w[1] is identical to
	  // ten2mk128[ind -1].w[1]
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	}	// else the result is exact
      }
      if (x_sign)
	res = -Cstar;
      else
	res = Cstar;
    } else if (exp == 0) {
      // 1 <= q <= 16
      // res = +/-C (exact)
      if (x_sign)
	res = -C1;
      else
	res = C1;
    } else {	// if (exp > 0) => 1 <= exp <= 18, 1 <= q <= 16, 2 <= q + exp <= 20
      // (the upper limit of 20 on q + exp is due to the fact that 
      // +/-C * 10^exp is guaranteed to fit in 64 bits) 
      // res = +/-C * 10^exp (exact)
      if (x_sign)
	res = -C1 * ten2k64[exp];
      else
	res = C1 * ten2k64[exp];
    }
  }
  BID_RETURN (res);
}

/*****************************************************************************
 *  BID64_to_int64_rninta
 ****************************************************************************/

#if DECIMAL_CALL_BY_REFERENCE
void
bid64_to_int64_rninta (SINT64 * pres, UINT64 * px
		       _EXC_FLAGS_PARAM _EXC_MASKS_PARAM
		       _EXC_INFO_PARAM) {
  UINT64 x = *px;
#else
SINT64
bid64_to_int64_rninta (UINT64 x
		       _EXC_FLAGS_PARAM _EXC_MASKS_PARAM
		       _EXC_INFO_PARAM) {
#endif
  SINT64 res;
  UINT64 x_sign;
  UINT64 x_exp;
  int exp;			// unbiased exponent
  // Note: C1 represents x_significand (UINT64)
  BID_UI64DOUBLE tmp1;
  unsigned int x_nr_bits;
  int q, ind, shift;
  UINT64 C1;
  UINT128 C;
  UINT64 Cstar;			// C* represents up to 16 decimal digits ~ 54 bits
  UINT128 P128;

  // check for NaN or Infinity
  if ((x & MASK_NAN) == MASK_NAN || (x & MASK_INF) == MASK_INF) {
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  }
  // unpack x
  x_sign = x & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
  // if steering bits are 11 (condition will be 0), then exponent is G[0:w+1] =>
  if ((x & MASK_STEERING_BITS) == MASK_STEERING_BITS) {
    x_exp = (x & MASK_BINARY_EXPONENT2) >> 51;	// biased
    C1 = (x & MASK_BINARY_SIG2) | MASK_BINARY_OR2;
    if (C1 > 9999999999999999ull) {	// non-canonical
      x_exp = 0;
      C1 = 0;
    }
  } else {
    x_exp = (x & MASK_BINARY_EXPONENT1) >> 53;	// biased
    C1 = x & MASK_BINARY_SIG1;
  }

  // check for zeros (possibly from non-canonical values)
  if (C1 == 0x0ull) {
    // x is 0
    res = 0x00000000;
    BID_RETURN (res);
  }
  // x is not special and is not zero

  // q = nr. of decimal digits in x (1 <= q <= 54)
  //  determine first the nr. of bits in x
  if (C1 >= 0x0020000000000000ull) {	// x >= 2^53
    // split the 64-bit value in two 32-bit halves to avoid rounding errors
    if (C1 >= 0x0000000100000000ull) {	// x >= 2^32
      tmp1.d = (double) (C1 >> 32);	// exact conversion
      x_nr_bits =
	33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    } else {	// x < 2^32
      tmp1.d = (double) C1;	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// if x < 2^53
    tmp1.d = (double) C1;	// exact conversion
    x_nr_bits =
      1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1 >= nr_digits[x_nr_bits - 1].threshold_lo)
      q++;
  }
  exp = x_exp - 398;	// unbiased exponent

  if ((q + exp) > 19) {	// x >= 10^19 ~= 2^63.11... (cannot fit in SINT64)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  } else if ((q + exp) == 19) {	// x = c(0)c(1)...c(18).c(19)...c(q-1)
    // in this case 2^63.11... ~= 10^19 <= x < 10^20 ~= 2^66.43...
    // so x rounded to an integer may or may not fit in a signed 64-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 19'
    if (x_sign) {	// if n < 0 and q + exp = 19
      // if n <= -2^63 - 1/2 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) >= 2^63+1/2
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 >= 5*(2^64+1), 1<=q<=16
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 >= 0x50000000000000005, 1<=q<=16
      // <=> C * 10^(20-q) >= 0x50000000000000005, 1<=q<=16
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      // Note: C1 * 10^(11-q) has 19 or 20 digits; 0x50000000000000005, has 20
      if (C.w[1] > 0x05ull || (C.w[1] == 0x05ull && C.w[0] >= 0x05ull)) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    } else {	// if n > 0 and q + exp = 19
      // if n >= 2^63 - 1/2 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) >= 2^63-1/2
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 >= 5*(2^64-1), 1<=q<=16
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 >= 0x4fffffffffffffffb, 1<=q<=16
      // <=> if C * 10^(20-q) >= 0x4fffffffffffffffb, 1<=q<=16
      C.w[1] = 0x0000000000000004ull;
      C.w[0] = 0xfffffffffffffffbull;
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      if (C.w[1] > 0x04ull ||
	  (C.w[1] == 0x04ull && C.w[0] >= 0xfffffffffffffffbull)) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    }	// end else if n > 0 and q + exp = 19
  }	// end else if ((q + exp) == 19)

  // n is not too large to be converted to int64: -2^63-1/2 < n < 2^63-1/2
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) < 0) {	// n = +/-0.0...c(0)c(1)...c(q-1)
    // return 0
    res = 0x0000000000000000ull;
    BID_RETURN (res);
  } else if ((q + exp) == 0) {	// n = +/-0.c(0)c(1)...c(q-1)
    // if 0.c(0)c(1)...c(q-1) <= 0.5 <=> c(0)c(1)...c(q-1) <= 5 * 10^(q-1)
    //   res = 0
    // else
    //   res = +/-1
    ind = q - 1;	// 0 <= ind <= 15
    if (C1 < midpoint64[ind]) {
      res = 0x0000000000000000ull;	// return 0
    } else if (x_sign) {	// n < 0
      res = 0xffffffffffffffffull;	// return -1
    } else {	// n > 0
      res = 0x0000000000000001ull;	// return +1
    }
  } else {	// if (1 <= q + exp <= 19, 1 <= q <= 16, -15 <= exp <= 18)
    // -2^63-1/2 < x <= -1 or 1 <= x < 2^63-1/2 so x can be rounded
    // to nearest to a 64-bit signed integer
    if (exp < 0) {	// 2 <= q <= 16, -15 <= exp <= -1, 1 <= q + exp <= 19
      ind = -exp;	// 1 <= ind <= 15; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 = C1 + 1/2 * 10^ind where the result C1 fits in 64 bits
      C1 = C1 + midpoint64[ind - 1];
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 15 
      // kx = 10^(-x) = ten2mk64[ind - 1]
      // C* = (C1 + 1/2 * 10^x) * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 54 bits
      __mul_64x64_to_128MACH (P128, C1, ten2mk64[ind - 1]);
      Cstar = P128.w[1];
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind].w[0], e.g.
      // if x=1, T*=ten2mk128trunc[0].w[0]=0x1999999999999999
      // if (0 < f* < 10^(-x)) then the result is a midpoint
      //   if floor(C*) is even then C* = floor(C*) - logical right
      //       shift; C* has p decimal digits, correct by Prop. 1)
      //   else if floor(C*) is odd C* = floor(C*)-1 (logical right
      //       shift; C* has p decimal digits, correct by Pr. 1)
      // else
      //   C* = floor(C*) (logical right shift; C has p decimal digits,
      //       correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-64 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 39
      Cstar = Cstar >> shift;

      // if the result was a midpoint it was rounded away from zero
      if (x_sign)
	res = -Cstar;
      else
	res = Cstar;
    } else if (exp == 0) {
      // 1 <= q <= 16
      // res = +/-C (exact)
      if (x_sign)
	res = -C1;
      else
	res = C1;
    } else {	// if (exp > 0) => 1 <= exp <= 18, 1 <= q <= 16, 2 <= q + exp <= 20
      // (the upper limit of 20 on q + exp is due to the fact that 
      // +/-C * 10^exp is guaranteed to fit in 64 bits) 
      // res = +/-C * 10^exp (exact)
      if (x_sign)
	res = -C1 * ten2k64[exp];
      else
	res = C1 * ten2k64[exp];
    }
  }
  BID_RETURN (res);
}

/*****************************************************************************
 *  BID64_to_int64_xrninta
 ****************************************************************************/

#if DECIMAL_CALL_BY_REFERENCE
void
bid64_to_int64_xrninta (SINT64 * pres, UINT64 * px
			_EXC_FLAGS_PARAM _EXC_MASKS_PARAM
			_EXC_INFO_PARAM) {
  UINT64 x = *px;
#else
SINT64
bid64_to_int64_xrninta (UINT64 x
			_EXC_FLAGS_PARAM _EXC_MASKS_PARAM
			_EXC_INFO_PARAM) {
#endif
  SINT64 res;
  UINT64 x_sign;
  UINT64 x_exp;
  int exp;			// unbiased exponent
  // Note: C1 represents x_significand (UINT64)
  UINT64 tmp64;
  BID_UI64DOUBLE tmp1;
  unsigned int x_nr_bits;
  int q, ind, shift;
  UINT64 C1;
  UINT128 C;
  UINT64 Cstar;			// C* represents up to 16 decimal digits ~ 54 bits
  UINT128 fstar;
  UINT128 P128;

  // check for NaN or Infinity
  if ((x & MASK_NAN) == MASK_NAN || (x & MASK_INF) == MASK_INF) {
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  }
  // unpack x
  x_sign = x & MASK_SIGN;	// 0 for positive, MASK_SIGN for negative
  // if steering bits are 11 (condition will be 0), then exponent is G[0:w+1] =>
  if ((x & MASK_STEERING_BITS) == MASK_STEERING_BITS) {
    x_exp = (x & MASK_BINARY_EXPONENT2) >> 51;	// biased
    C1 = (x & MASK_BINARY_SIG2) | MASK_BINARY_OR2;
    if (C1 > 9999999999999999ull) {	// non-canonical
      x_exp = 0;
      C1 = 0;
    }
  } else {
    x_exp = (x & MASK_BINARY_EXPONENT1) >> 53;	// biased
    C1 = x & MASK_BINARY_SIG1;
  }

  // check for zeros (possibly from non-canonical values)
  if (C1 == 0x0ull) {
    // x is 0
    res = 0x00000000;
    BID_RETURN (res);
  }
  // x is not special and is not zero

  // q = nr. of decimal digits in x (1 <= q <= 54)
  //  determine first the nr. of bits in x
  if (C1 >= 0x0020000000000000ull) {	// x >= 2^53
    // split the 64-bit value in two 32-bit halves to avoid rounding errors
    if (C1 >= 0x0000000100000000ull) {	// x >= 2^32
      tmp1.d = (double) (C1 >> 32);	// exact conversion
      x_nr_bits =
	33 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    } else {	// x < 2^32
      tmp1.d = (double) C1;	// exact conversion
      x_nr_bits =
	1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
    }
  } else {	// if x < 2^53
    tmp1.d = (double) C1;	// exact conversion
    x_nr_bits =
      1 + ((((unsigned int) (tmp1.ui64 >> 52)) & 0x7ff) - 0x3ff);
  }
  q = nr_digits[x_nr_bits - 1].digits;
  if (q == 0) {
    q = nr_digits[x_nr_bits - 1].digits1;
    if (C1 >= nr_digits[x_nr_bits - 1].threshold_lo)
      q++;
  }
  exp = x_exp - 398;	// unbiased exponent

  if ((q + exp) > 19) {	// x >= 10^19 ~= 2^63.11... (cannot fit in SINT64)
    // set invalid flag
    *pfpsf |= INVALID_EXCEPTION;
    // return Integer Indefinite
    res = 0x8000000000000000ull;
    BID_RETURN (res);
  } else if ((q + exp) == 19) {	// x = c(0)c(1)...c(18).c(19)...c(q-1)
    // in this case 2^63.11... ~= 10^19 <= x < 10^20 ~= 2^66.43...
    // so x rounded to an integer may or may not fit in a signed 64-bit int
    // the cases that do not fit are identified here; the ones that fit
    // fall through and will be handled with other cases further,
    // under '1 <= q + exp <= 19'
    if (x_sign) {	// if n < 0 and q + exp = 19
      // if n <= -2^63 - 1/2 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) >= 2^63+1/2
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 >= 5*(2^64+1), 1<=q<=16
      // <=> 0.c(0)c(1)...c(q-1) * 10^20 >= 0x50000000000000005, 1<=q<=16
      // <=> C * 10^(20-q) >= 0x50000000000000005, 1<=q<=16
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      // Note: C1 * 10^(11-q) has 19 or 20 digits; 0x50000000000000005, has 20
      if (C.w[1] > 0x05ull || (C.w[1] == 0x05ull && C.w[0] >= 0x05ull)) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    } else {	// if n > 0 and q + exp = 19
      // if n >= 2^63 - 1/2 then n is too large
      // too large if c(0)c(1)...c(18).c(19)...c(q-1) >= 2^63-1/2
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 >= 5*(2^64-1), 1<=q<=16
      // <=> if 0.c(0)c(1)...c(q-1) * 10^20 >= 0x4fffffffffffffffb, 1<=q<=16
      // <=> if C * 10^(20-q) >= 0x4fffffffffffffffb, 1<=q<=16
      C.w[1] = 0x0000000000000004ull;
      C.w[0] = 0xfffffffffffffffbull;
      // 1 <= q <= 16 => 4 <= 20-q <= 19 => 10^(20-q) is 64-bit, and so is C1
      __mul_64x64_to_128MACH (C, C1, ten2k64[20 - q]);
      if (C.w[1] > 0x04ull ||
	  (C.w[1] == 0x04ull && C.w[0] >= 0xfffffffffffffffbull)) {
	// set invalid flag
	*pfpsf |= INVALID_EXCEPTION;
	// return Integer Indefinite
	res = 0x8000000000000000ull;
	BID_RETURN (res);
      }
      // else cases that can be rounded to a 64-bit int fall through
      // to '1 <= q + exp <= 19'
    }	// end else if n > 0 and q + exp = 19
  }	// end else if ((q + exp) == 19)

  // n is not too large to be converted to int64: -2^63-1/2 < n < 2^63-1/2
  // Note: some of the cases tested for above fall through to this point
  if ((q + exp) < 0) {	// n = +/-0.0...c(0)c(1)...c(q-1)
    // set inexact flag
    *pfpsf |= INEXACT_EXCEPTION;
    // return 0
    res = 0x0000000000000000ull;
    BID_RETURN (res);
  } else if ((q + exp) == 0) {	// n = +/-0.c(0)c(1)...c(q-1)
    // if 0.c(0)c(1)...c(q-1) <= 0.5 <=> c(0)c(1)...c(q-1) <= 5 * 10^(q-1)
    //   res = 0
    // else
    //   res = +/-1
    ind = q - 1;	// 0 <= ind <= 15
    if (C1 < midpoint64[ind]) {
      res = 0x0000000000000000ull;	// return 0
    } else if (x_sign) {	// n < 0
      res = 0xffffffffffffffffull;	// return -1
    } else {	// n > 0
      res = 0x0000000000000001ull;	// return +1
    }
    // set inexact flag
    *pfpsf |= INEXACT_EXCEPTION;
  } else {	// if (1 <= q + exp <= 19, 1 <= q <= 16, -15 <= exp <= 18)
    // -2^63-1/2 < x <= -1 or 1 <= x < 2^63-1/2 so x can be rounded
    // to nearest to a 64-bit signed integer
    if (exp < 0) {	// 2 <= q <= 16, -15 <= exp <= -1, 1 <= q + exp <= 19
      ind = -exp;	// 1 <= ind <= 15; ind is a synonym for 'x'
      // chop off ind digits from the lower part of C1
      // C1 = C1 + 1/2 * 10^ind where the result C1 fits in 64 bits
      C1 = C1 + midpoint64[ind - 1];
      // calculate C* and f*
      // C* is actually floor(C*) in this case
      // C* and f* need shifting and masking, as shown by
      // shiftright128[] and maskhigh128[]
      // 1 <= x <= 15 
      // kx = 10^(-x) = ten2mk64[ind - 1]
      // C* = (C1 + 1/2 * 10^x) * 10^(-x)
      // the approximation of 10^(-x) was rounded up to 54 bits
      __mul_64x64_to_128MACH (P128, C1, ten2mk64[ind - 1]);
      Cstar = P128.w[1];
      fstar.w[1] = P128.w[1] & maskhigh128[ind - 1];
      fstar.w[0] = P128.w[0];
      // the top Ex bits of 10^(-x) are T* = ten2mk128trunc[ind].w[0], e.g.
      // if x=1, T*=ten2mk128trunc[0].w[0]=0x1999999999999999
      // if (0 < f* < 10^(-x)) then the result is a midpoint
      //   if floor(C*) is even then C* = floor(C*) - logical right
      //       shift; C* has p decimal digits, correct by Prop. 1)
      //   else if floor(C*) is odd C* = floor(C*)-1 (logical right
      //       shift; C* has p decimal digits, correct by Pr. 1)
      // else
      //   C* = floor(C*) (logical right shift; C has p decimal digits,
      //       correct by Property 1)
      // n = C* * 10^(e+x)

      // shift right C* by Ex-64 = shiftright128[ind]
      shift = shiftright128[ind - 1];	// 0 <= shift <= 39
      Cstar = Cstar >> shift;
      // determine inexactness of the rounding of C*
      // if (0 < f* - 1/2 < 10^(-x)) then
      //   the result is exact
      // else // if (f* - 1/2 > T*) then
      //   the result is inexact
      if (ind - 1 <= 2) {
	if (fstar.w[0] > 0x8000000000000000ull) {
	  // f* > 1/2 and the result may be exact
	  tmp64 = fstar.w[0] - 0x8000000000000000ull;	// f* - 1/2
	  if ((tmp64 > ten2mk128trunc[ind - 1].w[1])) {
	    // ten2mk128trunc[ind -1].w[1] is identical to 
	    // ten2mk128[ind -1].w[1]
	    // set the inexact flag
	    *pfpsf |= INEXACT_EXCEPTION;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	}
      } else {	// if 3 <= ind - 1 <= 14
	if (fstar.w[1] > onehalf128[ind - 1] ||
	    (fstar.w[1] == onehalf128[ind - 1] && fstar.w[0])) {
	  // f2* > 1/2 and the result may be exact
	  // Calculate f2* - 1/2
	  tmp64 = fstar.w[1] - onehalf128[ind - 1];
	  if (tmp64 || fstar.w[0] > ten2mk128trunc[ind - 1].w[1]) {
	    // ten2mk128trunc[ind -1].w[1] is identical to 
	    // ten2mk128[ind -1].w[1]
	    // set the inexact flag
	    *pfpsf |= INEXACT_EXCEPTION;
	  }	// else the result is exact
	} else {	// the result is inexact; f2* <= 1/2
	  // set the inexact flag
	  *pfpsf |= INEXACT_EXCEPTION;
	}
      }

      // if the result was a midpoint it was rounded away from zero
      if (x_sign)
	res = -Cstar;
      else
	res = Cstar;
    } else if (exp == 0) {
      // 1 <= q <= 16
      // res = +/-C (exact)
      if (x_sign)
	res = -C1;
      else
	res = C1;
    } else {	// if (exp > 0) => 1 <= exp <= 18, 1 <= q <= 16, 2 <= q + exp <= 20
      // (the upper limit of 20 on q + exp is due to the fact that 
      // +/-C * 10^exp is guaranteed to fit in 64 bits) 
      // res = +/-C * 10^exp (exact)
      if (x_sign)
	res = -C1 * ten2k64[exp];
      else
	res = C1 * ten2k64[exp];
    }
  }
  BID_RETURN (res);
}
