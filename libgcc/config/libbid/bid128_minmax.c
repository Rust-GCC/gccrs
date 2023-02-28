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

#define BID_128RES
#include "bid_internal.h"

/*****************************************************************************
 *  BID128 minimum number
 *****************************************************************************/

#if DECIMAL_CALL_BY_REFERENCE
void
bid128_minnum (UINT128 * pres, UINT128 * px,
	       UINT128 * py _EXC_FLAGS_PARAM) {
  UINT128 x = *px;
  UINT128 y = *py;
#else
UINT128
bid128_minnum (UINT128 x, UINT128 y _EXC_FLAGS_PARAM) {
#endif

  UINT128 res;
  int exp_x, exp_y;
  int diff;
  UINT128 sig_x, sig_y;
  UINT192 sig_n_prime192;
  UINT256 sig_n_prime256;
  char x_is_zero = 0, y_is_zero = 0;

  BID_SWAP128 (x);
  BID_SWAP128 (y);

  // check for non-canonical x
  if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
    x.w[1] = x.w[1] & 0xfe003fffffffffffull;	// clear out G[6]-G[16]
    // check for non-canonical NaN payload
    if (((x.w[1] & 0x00003fffffffffffull) > 0x0000314dc6448d93ull) ||
	(((x.w[1] & 0x00003fffffffffffull) == 0x0000314dc6448d93ull) &&
	 (x.w[0] > 0x38c15b09ffffffffull))) {
      x.w[1] = x.w[1] & 0xffffc00000000000ull;
      x.w[0] = 0x0ull;
    }
  } else if ((x.w[1] & MASK_ANY_INF) == MASK_INF) {	// x = inf
    x.w[1] = x.w[1] & (MASK_SIGN | MASK_INF);
    x.w[0] = 0x0ull;
  } else {	// x is not special
    // check for non-canonical values - treated as zero
    if ((x.w[1] & MASK_STEERING_BITS) == MASK_STEERING_BITS) {	// G0_G1=11
      // non-canonical
      x.w[1] = (x.w[1] & MASK_SIGN) | ((x.w[1] << 2) & MASK_EXP);
      x.w[0] = 0x0ull;
    } else {	// G0_G1 != 11
      if ((x.w[1] & MASK_COEFF) > 0x0001ed09bead87c0ull ||
	  ((x.w[1] & MASK_COEFF) == 0x0001ed09bead87c0ull
	   && x.w[0] > 0x378d8e63ffffffffull)) {
	// x is non-canonical if coefficient is larger than 10^34 -1
	x.w[1] = (x.w[1] & MASK_SIGN) | (x.w[1] & MASK_EXP);
	x.w[0] = 0x0ull;
      } else {	// canonical
	;
      }
    }
  }
  // check for non-canonical y
  if ((y.w[1] & MASK_NAN) == MASK_NAN) {	// y is NAN
    y.w[1] = y.w[1] & 0xfe003fffffffffffull;	// clear out G[6]-G[16]
    // check for non-canonical NaN payload
    if (((y.w[1] & 0x00003fffffffffffull) > 0x0000314dc6448d93ull) ||
	(((y.w[1] & 0x00003fffffffffffull) == 0x0000314dc6448d93ull) &&
	 (y.w[0] > 0x38c15b09ffffffffull))) {
      y.w[1] = y.w[1] & 0xffffc00000000000ull;
      y.w[0] = 0x0ull;
    }
  } else if ((y.w[1] & MASK_ANY_INF) == MASK_INF) {	// y = inf
    y.w[1] = y.w[1] & (MASK_SIGN | MASK_INF);
    y.w[0] = 0x0ull;
  } else {	// y is not special
    // check for non-canonical values - treated as zero
    if ((y.w[1] & MASK_STEERING_BITS) == MASK_STEERING_BITS) {	// G0_G1=11
      // non-canonical
      y.w[1] = (y.w[1] & MASK_SIGN) | ((y.w[1] << 2) & MASK_EXP);
      y.w[0] = 0x0ull;
    } else {	// G0_G1 != 11
      if ((y.w[1] & MASK_COEFF) > 0x0001ed09bead87c0ull ||
	  ((y.w[1] & MASK_COEFF) == 0x0001ed09bead87c0ull
	   && y.w[0] > 0x378d8e63ffffffffull)) {
	// y is non-canonical if coefficient is larger than 10^34 -1
	y.w[1] = (y.w[1] & MASK_SIGN) | (y.w[1] & MASK_EXP);
	y.w[0] = 0x0ull;
      } else {	// canonical
	;
      }
    }
  }

  // NaN (CASE1)
  if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
    if ((x.w[1] & MASK_SNAN) == MASK_SNAN) {	// x is SNaN
      // if x is SNAN, then return quiet (x)
      *pfpsf |= INVALID_EXCEPTION;	// set exception if SNaN
      x.w[1] = x.w[1] & 0xfdffffffffffffffull;	// quietize x
      res = x;
    } else {	// x is QNaN
      if ((y.w[1] & MASK_NAN) == MASK_NAN) {	// y is NAN
	if ((y.w[1] & MASK_SNAN) == MASK_SNAN) {	// y is SNAN
	  *pfpsf |= INVALID_EXCEPTION;	// set invalid flag
	}
	res = x;
      } else {
	res = y;
      }
    }
    BID_RETURN (res);
  } else if ((y.w[1] & MASK_NAN) == MASK_NAN) {	// y is NaN, but x is not
    if ((y.w[1] & MASK_SNAN) == MASK_SNAN) {
      *pfpsf |= INVALID_EXCEPTION;	// set exception if SNaN
      y.w[1] = y.w[1] & 0xfdffffffffffffffull;	// quietize y
      res = y;
    } else {
      // will return x (which is not NaN)
      res = x;
    }
    BID_RETURN (res);
  }
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal (not Greater).
  if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
    res = x;
    BID_RETURN (res);
  }
  // INFINITY (CASE3)
  if ((x.w[1] & MASK_INF) == MASK_INF) {
    // if x is neg infinity, there is no way it is greater than y, return 0
    res = (((x.w[1] & MASK_SIGN) == MASK_SIGN)) ? x : y;
    BID_RETURN (res);
  } else if ((y.w[1] & MASK_INF) == MASK_INF) {
    // x is finite, so if y is positive infinity, then x is less, return 0
    //                 if y is negative infinity, then x is greater, return 1
    res = ((y.w[1] & MASK_SIGN) == MASK_SIGN) ? y : x;
    BID_RETURN (res);
  }
  // CONVERT X
  sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
  sig_x.w[0] = x.w[0];
  exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CONVERT Y
  exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
  sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
  sig_y.w[0] = y.w[0];

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => ignore the exponent 
  //    field
  //    (Any non-canonical # is considered 0)
  if ((sig_x.w[1] == 0) && (sig_x.w[0] == 0)) {
    x_is_zero = 1;
  }
  if ((sig_y.w[1] == 0) && (sig_y.w[0] == 0)) {
    y_is_zero = 1;
  }

  if (x_is_zero && y_is_zero) {
    // if both numbers are zero, neither is greater => return either number
    res = x;
    BID_RETURN (res);
  } else if (x_is_zero) {
    // is x is zero, it is greater if Y is negative
    res = ((y.w[1] & MASK_SIGN) == MASK_SIGN) ? y : x;
    BID_RETURN (res);
  } else if (y_is_zero) {
    // is y is zero, X is greater if it is positive
    res = ((x.w[1] & MASK_SIGN) != MASK_SIGN) ? y : x;
    BID_RETURN (res);
  }
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
  if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
    res = ((y.w[1] & MASK_SIGN) == MASK_SIGN) ? y : x;
    BID_RETURN (res);
  }
  // REDUNDANT REPRESENTATIONS (CASE6)
  // if exponents are the same, then we have a simple comparison of 
  //    the significands
  if (exp_y == exp_x) {
    res = (((sig_x.w[1] > sig_y.w[1])
	    || (sig_x.w[1] == sig_y.w[1]
		&& sig_x.w[0] >= sig_y.w[0])) ^ ((x.w[1] & MASK_SIGN) ==
						 MASK_SIGN)) ? y : x;
    BID_RETURN (res);
  }
  // if both components are either bigger or smaller, it is clear what 
  //    needs to be done
  if (sig_x.w[1] >= sig_y.w[1] && sig_x.w[0] >= sig_y.w[0]
      && exp_x > exp_y) {
    res = ((x.w[1] & MASK_SIGN) != MASK_SIGN) ? y : x;
    BID_RETURN (res);
  }
  if (sig_x.w[1] <= sig_y.w[1] && sig_x.w[0] <= sig_y.w[0]
      && exp_x < exp_y) {
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN) ? y : x;
    BID_RETURN (res);
  }

  diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
  if (diff > 0) {	// to simplify the loop below,
    // if exp_x is 33 greater than exp_y, no need for compensation
    if (diff > 33) {
      // difference cannot be greater than 10^33
      res = ((x.w[1] & MASK_SIGN) != MASK_SIGN) ? y : x;
      BID_RETURN (res);
    }
    if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
      __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);
      // if postitive, return whichever significand is larger 
      // (converse if negative)
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) ==
				  MASK_SIGN)) ? y : x;
      BID_RETURN (res);
    }
    __mul_64x128_to_192 (sig_n_prime192, ten2k64[diff], sig_x);
    // if postitive, return whichever significand is larger 
    // (converse if negative)
    res =
      (((sig_n_prime192.w[2] > 0) || (sig_n_prime192.w[1] > sig_y.w[1])
	|| (sig_n_prime192.w[1] == sig_y.w[1]
	    && sig_n_prime192.w[0] >
	    sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN)) ? y : x;
    BID_RETURN (res);
  }
  diff = exp_y - exp_x;
  // if exp_x is 33 less than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN) ? y : x;
    BID_RETURN (res);
  }
  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    // adjust the y significand upwards
    __mul_128x128_to_256 (sig_n_prime256, sig_y, ten2k128[diff - 20]);
    // if postitive, return whichever significand is larger 
    // (converse if negative)
    res =
      ((sig_n_prime256.w[3] != 0 || sig_n_prime256.w[2] != 0
	|| (sig_n_prime256.w[1] > sig_x.w[1]
	    || (sig_n_prime256.w[1] == sig_x.w[1]
		&& sig_n_prime256.w[0] >
		sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) ==
				 MASK_SIGN)) ? x : y;
    BID_RETURN (res);
  }
  // adjust the y significand upwards
  __mul_64x128_to_192 (sig_n_prime192, ten2k64[diff], sig_y);
  // if postitive, return whichever significand is larger (converse if negative)
  res =
    ((sig_n_prime192.w[2] != 0
      || (sig_n_prime192.w[1] > sig_x.w[1]
	  || (sig_n_prime192.w[1] == sig_x.w[1]
	      && sig_n_prime192.w[0] > sig_x.w[0])))
     ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN)) ? x : y;
  BID_RETURN (res);
}

/*****************************************************************************
 *  BID128 minimum magnitude function - returns greater of two numbers
 *****************************************************************************/

#if DECIMAL_CALL_BY_REFERENCE
void
bid128_minnum_mag (UINT128 * pres, UINT128 * px,
		   UINT128 * py _EXC_FLAGS_PARAM) {
  UINT128 x = *px;
  UINT128 y = *py;
#else
UINT128
bid128_minnum_mag (UINT128 x, UINT128 y _EXC_FLAGS_PARAM) {
#endif

  UINT128 res;
  int exp_x, exp_y;
  int diff;
  UINT128 sig_x, sig_y;
  UINT192 sig_n_prime192;
  UINT256 sig_n_prime256;

  BID_SWAP128 (x);
  BID_SWAP128 (y);

  // check for non-canonical x
  if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
    x.w[1] = x.w[1] & 0xfe003fffffffffffull;	// clear out G[6]-G[16]
    // check for non-canonical NaN payload
    if (((x.w[1] & 0x00003fffffffffffull) > 0x0000314dc6448d93ull) ||
	(((x.w[1] & 0x00003fffffffffffull) == 0x0000314dc6448d93ull) &&
	 (x.w[0] > 0x38c15b09ffffffffull))) {
      x.w[1] = x.w[1] & 0xffffc00000000000ull;
      x.w[0] = 0x0ull;
    }
  } else if ((x.w[1] & MASK_ANY_INF) == MASK_INF) {	// x = inf
    x.w[1] = x.w[1] & (MASK_SIGN | MASK_INF);
    x.w[0] = 0x0ull;
  } else {	// x is not special
    // check for non-canonical values - treated as zero
    if ((x.w[1] & MASK_STEERING_BITS) == MASK_STEERING_BITS) {	// G0_G1=11
      // non-canonical
      x.w[1] = (x.w[1] & MASK_SIGN) | ((x.w[1] << 2) & MASK_EXP);
      x.w[0] = 0x0ull;
    } else {	// G0_G1 != 11
      if ((x.w[1] & MASK_COEFF) > 0x0001ed09bead87c0ull ||
	  ((x.w[1] & MASK_COEFF) == 0x0001ed09bead87c0ull
	   && x.w[0] > 0x378d8e63ffffffffull)) {
	// x is non-canonical if coefficient is larger than 10^34 -1
	x.w[1] = (x.w[1] & MASK_SIGN) | (x.w[1] & MASK_EXP);
	x.w[0] = 0x0ull;
      } else {	// canonical
	;
      }
    }
  }
  // check for non-canonical y
  if ((y.w[1] & MASK_NAN) == MASK_NAN) {	// y is NAN
    y.w[1] = y.w[1] & 0xfe003fffffffffffull;	// clear out G[6]-G[16]
    // check for non-canonical NaN payload
    if (((y.w[1] & 0x00003fffffffffffull) > 0x0000314dc6448d93ull) ||
	(((y.w[1] & 0x00003fffffffffffull) == 0x0000314dc6448d93ull) &&
	 (y.w[0] > 0x38c15b09ffffffffull))) {
      y.w[1] = y.w[1] & 0xffffc00000000000ull;
      y.w[0] = 0x0ull;
    }
  } else if ((y.w[1] & MASK_ANY_INF) == MASK_INF) {	// y = inf
    y.w[1] = y.w[1] & (MASK_SIGN | MASK_INF);
    y.w[0] = 0x0ull;
  } else {	// y is not special
    // check for non-canonical values - treated as zero
    if ((y.w[1] & MASK_STEERING_BITS) == MASK_STEERING_BITS) {	// G0_G1=11
      // non-canonical
      y.w[1] = (y.w[1] & MASK_SIGN) | ((y.w[1] << 2) & MASK_EXP);
      y.w[0] = 0x0ull;
    } else {	// G0_G1 != 11
      if ((y.w[1] & MASK_COEFF) > 0x0001ed09bead87c0ull ||
	  ((y.w[1] & MASK_COEFF) == 0x0001ed09bead87c0ull
	   && y.w[0] > 0x378d8e63ffffffffull)) {
	// y is non-canonical if coefficient is larger than 10^34 -1
	y.w[1] = (y.w[1] & MASK_SIGN) | (y.w[1] & MASK_EXP);
	y.w[0] = 0x0ull;
      } else {	// canonical
	;
      }
    }
  }

  // NaN (CASE1)
  if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
    if ((x.w[1] & MASK_SNAN) == MASK_SNAN) {	// x is SNaN
      // if x is SNAN, then return quiet (x)
      *pfpsf |= INVALID_EXCEPTION;	// set exception if SNaN
      x.w[1] = x.w[1] & 0xfdffffffffffffffull;	// quietize x
      res = x;
    } else {	// x is QNaN
      if ((y.w[1] & MASK_NAN) == MASK_NAN) {	// y is NAN
	if ((y.w[1] & MASK_SNAN) == MASK_SNAN) {	// y is SNAN
	  *pfpsf |= INVALID_EXCEPTION;	// set invalid flag
	}
	res = x;
      } else {
	res = y;
      }
    }
    BID_RETURN (res);
  } else if ((y.w[1] & MASK_NAN) == MASK_NAN) {	// y is NaN, but x is not
    if ((y.w[1] & MASK_SNAN) == MASK_SNAN) {
      *pfpsf |= INVALID_EXCEPTION;	// set exception if SNaN
      y.w[1] = y.w[1] & 0xfdffffffffffffffull;	// quietize y
      res = y;
    } else {
      // will return x (which is not NaN)
      res = x;
    }
    BID_RETURN (res);
  }
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal (not Greater).
  if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
    res = y;
    BID_RETURN (res);
  }
  // INFINITY (CASE3)
  if ((x.w[1] & MASK_INF) == MASK_INF) {
    // if x infinity, it has maximum magnitude.
    // Check if magnitudes are equal.  If x is negative, return it.
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN
	   && (y.w[1] & MASK_INF) == MASK_INF) ? x : y;
    BID_RETURN (res);
  } else if ((y.w[1] & MASK_INF) == MASK_INF) {
    // x is finite, so if y is infinity, then x is less in magnitude
    res = x;
    BID_RETURN (res);
  }
  // CONVERT X
  sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
  sig_x.w[0] = x.w[0];
  exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CONVERT Y
  exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
  sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
  sig_y.w[0] = y.w[0];

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => 
  //        therefore ignore the exponent field
  //    (Any non-canonical # is considered 0)
  if ((sig_x.w[1] == 0) && (sig_x.w[0] == 0)) {
    res = x;
    BID_RETURN (res);
  }
  if ((sig_y.w[1] == 0) && (sig_y.w[0] == 0)) {
    res = y;
    BID_RETURN (res);
  }
  // REDUNDANT REPRESENTATIONS (CASE6)
  // check if exponents are the same and significands are the same
  if (exp_y == exp_x && sig_x.w[1] == sig_y.w[1]
      && sig_x.w[0] == sig_y.w[0]) {
    if (x.w[1] & 0x8000000000000000ull) {	// x is negative
      res = x;
      BID_RETURN (res);
    } else {
      res = y;
      BID_RETURN (res);
    }
  } else if (((sig_x.w[1] > sig_y.w[1] || (sig_x.w[1] == sig_y.w[1]
					   && sig_x.w[0] > sig_y.w[0]))
	      && exp_x == exp_y)
	     || ((sig_x.w[1] > sig_y.w[1]
		  || (sig_x.w[1] == sig_y.w[1]
		      && sig_x.w[0] >= sig_y.w[0]))
		 && exp_x > exp_y)) {
    // if both components are either bigger or smaller, it is clear what 
    // needs to be done; also if the magnitudes are equal
    res = y;
    BID_RETURN (res);
  } else if (((sig_y.w[1] > sig_x.w[1] || (sig_y.w[1] == sig_x.w[1]
					   && sig_y.w[0] > sig_x.w[0]))
	      && exp_y == exp_x)
	     || ((sig_y.w[1] > sig_x.w[1]
		  || (sig_y.w[1] == sig_x.w[1]
		      && sig_y.w[0] >= sig_x.w[0]))
		 && exp_y > exp_x)) {
    res = x;
    BID_RETURN (res);
  } else {
    ;	// continue
  }
  diff = exp_x - exp_y;
  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
  if (diff > 0) {	// to simplify the loop below,
    // if exp_x is 33 greater than exp_y, no need for compensation
    if (diff > 33) {
      res = y;	// difference cannot be greater than 10^33
      BID_RETURN (res);
    }
    if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
      __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);
      // if positive, return whichever significand is larger 
      // (converse if negative)
      if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	  && sig_n_prime256.w[1] == sig_y.w[1]
	  && (sig_n_prime256.w[0] == sig_y.w[0])) {
	res = ((y.w[1] & MASK_SIGN) == MASK_SIGN) ? y : x;	// if equal
	BID_RETURN (res);
      }
      res = (((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	     || (sig_n_prime256.w[1] > sig_y.w[1])
	     || (sig_n_prime256.w[1] == sig_y.w[1]
		 && sig_n_prime256.w[0] > sig_y.w[0])) ? y : x;
      BID_RETURN (res);
    }
    __mul_64x128_to_192 (sig_n_prime192, ten2k64[diff], sig_x);
    // if positive, return whichever significand is larger 
    // (converse if negative)
    if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
	&& (sig_n_prime192.w[0] == sig_y.w[0])) {
      // if = in magnitude, return +, (if possible)
      res = ((y.w[1] & MASK_SIGN) == MASK_SIGN) ? y : x;
      BID_RETURN (res);
    }
    res = ((sig_n_prime192.w[2] > 0)
	   || (sig_n_prime192.w[1] > sig_y.w[1])
	   || (sig_n_prime192.w[1] == sig_y.w[1]
	       && sig_n_prime192.w[0] > sig_y.w[0])) ? y : x;
    BID_RETURN (res);
  }
  diff = exp_y - exp_x;
  // if exp_x is 33 less than exp_y, no need for compensation
  if (diff > 33) {
    res = x;
    BID_RETURN (res);
  }
  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    // adjust the y significand upwards
    __mul_128x128_to_256 (sig_n_prime256, sig_y, ten2k128[diff - 20]);
    // if positive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_x.w[1]
	&& (sig_n_prime256.w[0] == sig_x.w[0])) {
      // if = in magnitude, return +, (if possible)
      res = ((y.w[1] & MASK_SIGN) == MASK_SIGN) ? y : x;
      BID_RETURN (res);
    }
    res = (sig_n_prime256.w[3] == 0 && sig_n_prime256.w[2] == 0
	   && (sig_n_prime256.w[1] < sig_x.w[1]
	       || (sig_n_prime256.w[1] == sig_x.w[1]
		   && sig_n_prime256.w[0] < sig_x.w[0]))) ? y : x;
    BID_RETURN (res);
  }
  // adjust the y significand upwards
  __mul_64x128_to_192 (sig_n_prime192, ten2k64[diff], sig_y);
  // if positive, return whichever significand is larger (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
      && (sig_n_prime192.w[0] == sig_x.w[0])) {
    // if = in magnitude, return +, if possible)
    res = ((y.w[1] & MASK_SIGN) == MASK_SIGN) ? y : x;
    BID_RETURN (res);
  }
  res = (sig_n_prime192.w[2] == 0
	 && (sig_n_prime192.w[1] < sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] < sig_x.w[0]))) ? y : x;
  BID_RETURN (res);
}

/*****************************************************************************
 *  BID128 maximum function - returns greater of two numbers
 *****************************************************************************/

#if DECIMAL_CALL_BY_REFERENCE
void
bid128_maxnum (UINT128 * pres, UINT128 * px,
	       UINT128 * py _EXC_FLAGS_PARAM) {
  UINT128 x = *px;
  UINT128 y = *py;
#else
UINT128
bid128_maxnum (UINT128 x, UINT128 y _EXC_FLAGS_PARAM) {
#endif

  UINT128 res;
  int exp_x, exp_y;
  int diff;
  UINT128 sig_x, sig_y;
  UINT192 sig_n_prime192;
  UINT256 sig_n_prime256;
  char x_is_zero = 0, y_is_zero = 0;

  BID_SWAP128 (x);
  BID_SWAP128 (y);

  // check for non-canonical x
  if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
    x.w[1] = x.w[1] & 0xfe003fffffffffffull;	// clear out G[6]-G[16]
    // check for non-canonical NaN payload
    if (((x.w[1] & 0x00003fffffffffffull) > 0x0000314dc6448d93ull) ||
	(((x.w[1] & 0x00003fffffffffffull) == 0x0000314dc6448d93ull) &&
	 (x.w[0] > 0x38c15b09ffffffffull))) {
      x.w[1] = x.w[1] & 0xffffc00000000000ull;
      x.w[0] = 0x0ull;
    }
  } else if ((x.w[1] & MASK_ANY_INF) == MASK_INF) {	// x = inf
    x.w[1] = x.w[1] & (MASK_SIGN | MASK_INF);
    x.w[0] = 0x0ull;
  } else {	// x is not special
    // check for non-canonical values - treated as zero
    if ((x.w[1] & MASK_STEERING_BITS) == MASK_STEERING_BITS) {	// G0_G1=11
      // non-canonical
      x.w[1] = (x.w[1] & MASK_SIGN) | ((x.w[1] << 2) & MASK_EXP);
      x.w[0] = 0x0ull;
    } else {	// G0_G1 != 11
      if ((x.w[1] & MASK_COEFF) > 0x0001ed09bead87c0ull ||
	  ((x.w[1] & MASK_COEFF) == 0x0001ed09bead87c0ull
	   && x.w[0] > 0x378d8e63ffffffffull)) {
	// x is non-canonical if coefficient is larger than 10^34 -1
	x.w[1] = (x.w[1] & MASK_SIGN) | (x.w[1] & MASK_EXP);
	x.w[0] = 0x0ull;
      } else {	// canonical
	;
      }
    }
  }
  // check for non-canonical y
  if ((y.w[1] & MASK_NAN) == MASK_NAN) {	// y is NAN
    y.w[1] = y.w[1] & 0xfe003fffffffffffull;	// clear out G[6]-G[16]
    // check for non-canonical NaN payload
    if (((y.w[1] & 0x00003fffffffffffull) > 0x0000314dc6448d93ull) ||
	(((y.w[1] & 0x00003fffffffffffull) == 0x0000314dc6448d93ull) &&
	 (y.w[0] > 0x38c15b09ffffffffull))) {
      y.w[1] = y.w[1] & 0xffffc00000000000ull;
      y.w[0] = 0x0ull;
    }
  } else if ((y.w[1] & MASK_ANY_INF) == MASK_INF) {	// y = inf
    y.w[1] = y.w[1] & (MASK_SIGN | MASK_INF);
    y.w[0] = 0x0ull;
  } else {	// y is not special
    // check for non-canonical values - treated as zero
    if ((y.w[1] & MASK_STEERING_BITS) == MASK_STEERING_BITS) {	// G0_G1=11
      // non-canonical
      y.w[1] = (y.w[1] & MASK_SIGN) | ((y.w[1] << 2) & MASK_EXP);
      y.w[0] = 0x0ull;
    } else {	// G0_G1 != 11
      if ((y.w[1] & MASK_COEFF) > 0x0001ed09bead87c0ull ||
	  ((y.w[1] & MASK_COEFF) == 0x0001ed09bead87c0ull
	   && y.w[0] > 0x378d8e63ffffffffull)) {
	// y is non-canonical if coefficient is larger than 10^34 -1
	y.w[1] = (y.w[1] & MASK_SIGN) | (y.w[1] & MASK_EXP);
	y.w[0] = 0x0ull;
      } else {	// canonical
	;
      }
    }
  }

  // NaN (CASE1)
  if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
    if ((x.w[1] & MASK_SNAN) == MASK_SNAN) {	// x is SNaN
      // if x is SNAN, then return quiet (x)
      *pfpsf |= INVALID_EXCEPTION;	// set exception if SNaN
      x.w[1] = x.w[1] & 0xfdffffffffffffffull;	// quietize x
      res = x;
    } else {	// x is QNaN
      if ((y.w[1] & MASK_NAN) == MASK_NAN) {	// y is NAN
	if ((y.w[1] & MASK_SNAN) == MASK_SNAN) {	// y is SNAN
	  *pfpsf |= INVALID_EXCEPTION;	// set invalid flag
	}
	res = x;
      } else {
	res = y;
      }
    }
    BID_RETURN (res);
  } else if ((y.w[1] & MASK_NAN) == MASK_NAN) {	// y is NaN, but x is not
    if ((y.w[1] & MASK_SNAN) == MASK_SNAN) {
      *pfpsf |= INVALID_EXCEPTION;	// set exception if SNaN
      y.w[1] = y.w[1] & 0xfdffffffffffffffull;	// quietize y
      res = y;
    } else {
      // will return x (which is not NaN)
      res = x;
    }
    BID_RETURN (res);
  }
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal (not Greater).
  if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
    res = x;
    BID_RETURN (res);
  }
  // INFINITY (CASE3)
  if ((x.w[1] & MASK_INF) == MASK_INF) {
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN) ? y : x;
    BID_RETURN (res);
  } else if ((y.w[1] & MASK_INF) == MASK_INF) {
    // x is finite, so if y is positive infinity, then x is less, return 0
    //                 if y is negative infinity, then x is greater, return 1
    res = ((y.w[1] & MASK_SIGN) == MASK_SIGN) ? x : y;
    BID_RETURN (res);
  }
  // CONVERT X
  sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
  sig_x.w[0] = x.w[0];
  exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CONVERT Y
  exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
  sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
  sig_y.w[0] = y.w[0];

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => 
  //        therefore ignore the exponent field
  //    (Any non-canonical # is considered 0)
  if ((sig_x.w[1] == 0) && (sig_x.w[0] == 0)) {
    x_is_zero = 1;
  }
  if ((sig_y.w[1] == 0) && (sig_y.w[0] == 0)) {
    y_is_zero = 1;
  }

  if (x_is_zero && y_is_zero) {
    // if both numbers are zero, neither is greater => return either number
    res = x;
    BID_RETURN (res);
  } else if (x_is_zero) {
    // is x is zero, it is greater if Y is negative
    res = ((y.w[1] & MASK_SIGN) == MASK_SIGN) ? x : y;
    BID_RETURN (res);
  } else if (y_is_zero) {
    // is y is zero, X is greater if it is positive
    res = ((x.w[1] & MASK_SIGN) != MASK_SIGN) ? x : y;
    BID_RETURN (res);
  }
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
  if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
    res = ((y.w[1] & MASK_SIGN) == MASK_SIGN) ? x : y;
    BID_RETURN (res);
  }
  // REDUNDANT REPRESENTATIONS (CASE6)
  // if exponents are the same, then we have a simple comparison of 
  // the significands
  if (exp_y == exp_x) {
    res = (((sig_x.w[1] > sig_y.w[1]) || (sig_x.w[1] == sig_y.w[1] &&
					  sig_x.w[0] >= sig_y.w[0])) ^
	   ((x.w[1] & MASK_SIGN) == MASK_SIGN)) ? x : y;
    BID_RETURN (res);
  }
  // if both components are either bigger or smaller, it is clear what 
  // needs to be done
  if ((sig_x.w[1] > sig_y.w[1]
       || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] > sig_y.w[0]))
      && exp_x >= exp_y) {
    res = ((x.w[1] & MASK_SIGN) != MASK_SIGN) ? x : y;
    BID_RETURN (res);
  }
  if ((sig_x.w[1] < sig_y.w[1]
       || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] < sig_y.w[0]))
      && exp_x <= exp_y) {
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN) ? x : y;
    BID_RETURN (res);
  }
  diff = exp_x - exp_y;
  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
  if (diff > 0) {	// to simplify the loop below,
    // if exp_x is 33 greater than exp_y, no need for compensation
    if (diff > 33) {
      // difference cannot be greater than 10^33
      res = ((x.w[1] & MASK_SIGN) != MASK_SIGN) ? x : y;
      BID_RETURN (res);
    }
    if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
      __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);
      // if postitive, return whichever significand is larger 
      // (converse if negative)
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) ==
				  MASK_SIGN)) ? x : y;
      BID_RETURN (res);
    }
    __mul_64x128_to_192 (sig_n_prime192, ten2k64[diff], sig_x);
    // if postitive, return whichever significand is larger 
    // (converse if negative)
    res =
      (((sig_n_prime192.w[2] > 0) || (sig_n_prime192.w[1] > sig_y.w[1])
	|| (sig_n_prime192.w[1] == sig_y.w[1]
	    && sig_n_prime192.w[0] >
	    sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN)) ? x : y;
    BID_RETURN (res);
  }
  diff = exp_y - exp_x;
  // if exp_x is 33 less than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN) ? x : y;
    BID_RETURN (res);
  }
  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    // adjust the y significand upwards
    __mul_128x128_to_256 (sig_n_prime256, sig_y, ten2k128[diff - 20]);
    // if postitive, return whichever significand is larger 
    // (converse if negative)
    res =
      ((sig_n_prime256.w[3] != 0 || sig_n_prime256.w[2] != 0
	|| (sig_n_prime256.w[1] > sig_x.w[1]
	    || (sig_n_prime256.w[1] == sig_x.w[1]
		&& sig_n_prime256.w[0] >
		sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) !=
				 MASK_SIGN)) ? x : y;
    BID_RETURN (res);
  }
  // adjust the y significand upwards
  __mul_64x128_to_192 (sig_n_prime192, ten2k64[diff], sig_y);
  // if postitive, return whichever significand is larger (converse if negative)
  res =
    ((sig_n_prime192.w[2] != 0
      || (sig_n_prime192.w[1] > sig_x.w[1]
	  || (sig_n_prime192.w[1] == sig_x.w[1]
	      && sig_n_prime192.w[0] >
	      sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) !=
			       MASK_SIGN)) ? x : y;
  BID_RETURN (res);
}

/*****************************************************************************
 *  BID128 maximum magnitude function - returns greater of two numbers
 *****************************************************************************/

#if DECIMAL_CALL_BY_REFERENCE
void
bid128_maxnum_mag (UINT128 * pres, UINT128 * px,
		   UINT128 * py _EXC_FLAGS_PARAM) {
  UINT128 x = *px;
  UINT128 y = *py;
#else
UINT128
bid128_maxnum_mag (UINT128 x, UINT128 y _EXC_FLAGS_PARAM) {
#endif

  UINT128 res;
  int exp_x, exp_y;
  int diff;
  UINT128 sig_x, sig_y;
  UINT192 sig_n_prime192;
  UINT256 sig_n_prime256;

  BID_SWAP128 (x);
  BID_SWAP128 (y);

  // check for non-canonical x
  if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
    x.w[1] = x.w[1] & 0xfe003fffffffffffull;	// clear out G[6]-G[16]
    // check for non-canonical NaN payload
    if (((x.w[1] & 0x00003fffffffffffull) > 0x0000314dc6448d93ull) ||
	(((x.w[1] & 0x00003fffffffffffull) == 0x0000314dc6448d93ull) &&
	 (x.w[0] > 0x38c15b09ffffffffull))) {
      x.w[1] = x.w[1] & 0xffffc00000000000ull;
      x.w[0] = 0x0ull;
    }
  } else if ((x.w[1] & MASK_ANY_INF) == MASK_INF) {	// x = inf
    x.w[1] = x.w[1] & (MASK_SIGN | MASK_INF);
    x.w[0] = 0x0ull;
  } else {	// x is not special
    // check for non-canonical values - treated as zero
    if ((x.w[1] & MASK_STEERING_BITS) == MASK_STEERING_BITS) {	// G0_G1=11
      // non-canonical
      x.w[1] = (x.w[1] & MASK_SIGN) | ((x.w[1] << 2) & MASK_EXP);
      x.w[0] = 0x0ull;
    } else {	// G0_G1 != 11
      if ((x.w[1] & MASK_COEFF) > 0x0001ed09bead87c0ull ||
	  ((x.w[1] & MASK_COEFF) == 0x0001ed09bead87c0ull
	   && x.w[0] > 0x378d8e63ffffffffull)) {
	// x is non-canonical if coefficient is larger than 10^34 -1
	x.w[1] = (x.w[1] & MASK_SIGN) | (x.w[1] & MASK_EXP);
	x.w[0] = 0x0ull;
      } else {	// canonical
	;
      }
    }
  }
  // check for non-canonical y
  if ((y.w[1] & MASK_NAN) == MASK_NAN) {	// y is NAN
    y.w[1] = y.w[1] & 0xfe003fffffffffffull;	// clear out G[6]-G[16]
    // check for non-canonical NaN payload
    if (((y.w[1] & 0x00003fffffffffffull) > 0x0000314dc6448d93ull) ||
	(((y.w[1] & 0x00003fffffffffffull) == 0x0000314dc6448d93ull) &&
	 (y.w[0] > 0x38c15b09ffffffffull))) {
      y.w[1] = y.w[1] & 0xffffc00000000000ull;
      y.w[0] = 0x0ull;
    }
  } else if ((y.w[1] & MASK_ANY_INF) == MASK_INF) {	// y = inf
    y.w[1] = y.w[1] & (MASK_SIGN | MASK_INF);
    y.w[0] = 0x0ull;
  } else {	// y is not special
    // check for non-canonical values - treated as zero
    if ((y.w[1] & MASK_STEERING_BITS) == MASK_STEERING_BITS) {	// G0_G1=11
      // non-canonical
      y.w[1] = (y.w[1] & MASK_SIGN) | ((y.w[1] << 2) & MASK_EXP);
      y.w[0] = 0x0ull;
    } else {	// G0_G1 != 11
      if ((y.w[1] & MASK_COEFF) > 0x0001ed09bead87c0ull ||
	  ((y.w[1] & MASK_COEFF) == 0x0001ed09bead87c0ull &&
	   y.w[0] > 0x378d8e63ffffffffull)) {
	// y is non-canonical if coefficient is larger than 10^34 -1
	y.w[1] = (y.w[1] & MASK_SIGN) | (y.w[1] & MASK_EXP);
	y.w[0] = 0x0ull;
      } else {	// canonical
	;
      }
    }
  }

  // NaN (CASE1)
  if ((x.w[1] & MASK_NAN) == MASK_NAN) {	// x is NAN
    if ((x.w[1] & MASK_SNAN) == MASK_SNAN) {	// x is SNaN
      // if x is SNAN, then return quiet (x)
      *pfpsf |= INVALID_EXCEPTION;	// set exception if SNaN
      x.w[1] = x.w[1] & 0xfdffffffffffffffull;	// quietize x
      res = x;
    } else {	// x is QNaN
      if ((y.w[1] & MASK_NAN) == MASK_NAN) {	// y is NAN
	if ((y.w[1] & MASK_SNAN) == MASK_SNAN) {	// y is SNAN
	  *pfpsf |= INVALID_EXCEPTION;	// set invalid flag
	}
	res = x;
      } else {
	res = y;
      }
    }
    BID_RETURN (res);
  } else if ((y.w[1] & MASK_NAN) == MASK_NAN) {	// y is NaN, but x is not
    if ((y.w[1] & MASK_SNAN) == MASK_SNAN) {
      *pfpsf |= INVALID_EXCEPTION;	// set exception if SNaN
      y.w[1] = y.w[1] & 0xfdffffffffffffffull;	// quietize y
      res = y;
    } else {
      // will return x (which is not NaN)
      res = x;
    }
    BID_RETURN (res);
  }
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal (not Greater).
  if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
    res = y;
    BID_RETURN (res);
  }
  // INFINITY (CASE3)
  if ((x.w[1] & MASK_INF) == MASK_INF) {
    // if x infinity, it has maximum magnitude
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN
	   && (y.w[1] & MASK_INF) == MASK_INF) ? y : x;
    BID_RETURN (res);
  } else if ((y.w[1] & MASK_INF) == MASK_INF) {
    // x is finite, so if y is positive infinity, then x is less, return 0
    //                 if y is negative infinity, then x is greater, return 1
    res = y;
    BID_RETURN (res);
  }
  // CONVERT X
  sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
  sig_x.w[0] = x.w[0];
  exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CONVERT Y
  exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
  sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
  sig_y.w[0] = y.w[0];

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => 
  //         therefore ignore the exponent field
  //    (Any non-canonical # is considered 0)
  if ((sig_x.w[1] == 0) && (sig_x.w[0] == 0)) {
    res = y;
    BID_RETURN (res);
  }
  if ((sig_y.w[1] == 0) && (sig_y.w[0] == 0)) {
    res = x;
    BID_RETURN (res);
  }
  // REDUNDANT REPRESENTATIONS (CASE6)
  if (exp_y == exp_x && sig_x.w[1] == sig_y.w[1]
      && sig_x.w[0] == sig_y.w[0]) {
    // check if exponents are the same and significands are the same
    if (x.w[1] & 0x8000000000000000ull) {	// x is negative
      res = y;
      BID_RETURN (res);
    } else {
      res = x;
      BID_RETURN (res);
    }
  } else if (((sig_x.w[1] > sig_y.w[1] || (sig_x.w[1] == sig_y.w[1]
					   && sig_x.w[0] > sig_y.w[0]))
	      && exp_x == exp_y)
	     || ((sig_x.w[1] > sig_y.w[1]
		  || (sig_x.w[1] == sig_y.w[1]
		      && sig_x.w[0] >= sig_y.w[0]))
		 && exp_x > exp_y)) {
    // if both components are either bigger or smaller, it is clear what 
    // needs to be done; also if the magnitudes are equal
    res = x;
    BID_RETURN (res);
  } else if (((sig_y.w[1] > sig_x.w[1] || (sig_y.w[1] == sig_x.w[1]
					   && sig_y.w[0] > sig_x.w[0]))
	      && exp_y == exp_x)
	     || ((sig_y.w[1] > sig_x.w[1]
		  || (sig_y.w[1] == sig_x.w[1]
		      && sig_y.w[0] >= sig_x.w[0]))
		 && exp_y > exp_x)) {
    res = y;
    BID_RETURN (res);
  } else {
    ;	// continue
  }
  diff = exp_x - exp_y;
  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
  if (diff > 0) {	// to simplify the loop below,
    // if exp_x is 33 greater than exp_y, no need for compensation
    if (diff > 33) {
      res = x;	// difference cannot be greater than 10^33
      BID_RETURN (res);
    }
    if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
      __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);
      // if postitive, return whichever significand is larger 
      // (converse if negative)
      if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	  && sig_n_prime256.w[1] == sig_y.w[1]
	  && (sig_n_prime256.w[0] == sig_y.w[0])) {
	res = ((y.w[1] & MASK_SIGN) == MASK_SIGN) ? x : y;	// if equal
	BID_RETURN (res);
      }
      res = (((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	     || (sig_n_prime256.w[1] > sig_y.w[1])
	     || (sig_n_prime256.w[1] == sig_y.w[1]
		 && sig_n_prime256.w[0] > sig_y.w[0])) ? x : y;
      BID_RETURN (res);
    }
    __mul_64x128_to_192 (sig_n_prime192, ten2k64[diff], sig_x);
    // if postitive, return whichever significand is larger (converse if negative)
    if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
	&& (sig_n_prime192.w[0] == sig_y.w[0])) {
      // if equal, return positive magnitude
      res = ((y.w[1] & MASK_SIGN) == MASK_SIGN) ? x : y;
      BID_RETURN (res);
    }
    res = ((sig_n_prime192.w[2] > 0)
	   || (sig_n_prime192.w[1] > sig_y.w[1])
	   || (sig_n_prime192.w[1] == sig_y.w[1]
	       && sig_n_prime192.w[0] > sig_y.w[0])) ? x : y;
    BID_RETURN (res);
  }
  diff = exp_y - exp_x;
  // if exp_x is 33 less than exp_y, no need for compensation
  if (diff > 33) {
    res = y;
    BID_RETURN (res);
  }
  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    // adjust the y significand upwards
    __mul_128x128_to_256 (sig_n_prime256, sig_y, ten2k128[diff - 20]);
    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_x.w[1]
	&& (sig_n_prime256.w[0] == sig_x.w[0])) {
      // if equal, return positive (if possible)
      res = ((y.w[1] & MASK_SIGN) == MASK_SIGN) ? x : y;
      BID_RETURN (res);
    }
    res = (sig_n_prime256.w[3] == 0 && sig_n_prime256.w[2] == 0
	   && (sig_n_prime256.w[1] < sig_x.w[1]
	       || (sig_n_prime256.w[1] == sig_x.w[1]
		   && sig_n_prime256.w[0] < sig_x.w[0]))) ? x : y;
    BID_RETURN (res);
  }
  // adjust the y significand upwards
  __mul_64x128_to_192 (sig_n_prime192, ten2k64[diff], sig_y);
  // if postitive, return whichever significand is larger (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
      && (sig_n_prime192.w[0] == sig_x.w[0])) {
    // if equal, return positive (if possible)
    res = ((y.w[1] & MASK_SIGN) == MASK_SIGN) ? x : y;
    BID_RETURN (res);
  }
  res = (sig_n_prime192.w[2] == 0
	 && (sig_n_prime192.w[1] < sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] < sig_x.w[0]))) ? x : y;
  BID_RETURN (res);
}
