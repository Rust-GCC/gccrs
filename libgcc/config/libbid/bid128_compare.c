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

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int, bid128_quiet_equal, x, y)

     int res;
     int exp_x, exp_y, exp_t;
     UINT128 sig_x, sig_y, sig_t;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered, 
  // rather than equal : return 0
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
if ((x.w[1] & MASK_SNAN) == MASK_SNAN
    || (y.w[1] & MASK_SNAN) == MASK_SNAN) {
  *pfpsf |= INVALID_EXCEPTION;
}
{
  res = 0;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equivalent.
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 1;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  if ((y.w[1] & MASK_INF) == MASK_INF) {
    res = (((x.w[1] ^ y.w[1]) & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  } else {
    res = 0;
    BID_RETURN (res);
  }
}
if ((y.w[1] & MASK_INF) == MASK_INF) {
  res = 0;
  BID_RETURN (res);
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //   If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  // If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}

if (x_is_zero && y_is_zero) {
  res = 1;
  BID_RETURN (res);
} else if ((x_is_zero && !y_is_zero) || (!x_is_zero && y_is_zero)) {
  res = 0;
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ => not equal : return 0
if ((x.w[1] ^ y.w[1]) & MASK_SIGN) {
  res = 0;
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)
if (exp_x > exp_y) {	// to simplify the loop below,
  SWAP (exp_x, exp_y, exp_t);	// put the larger exp in y,
  SWAP (sig_x.w[1], sig_y.w[1], sig_t.w[1]);	// and the smaller exp in x
  SWAP (sig_x.w[0], sig_y.w[0], sig_t.w[0]);	// and the smaller exp in x
}


if (exp_y - exp_x > 33) {
  res = 0;
  BID_RETURN (res);
}	// difference cannot be greater than 10^33

if (exp_y - exp_x > 19) {
  // recalculate y's significand upwards
  __mul_128x128_to_256 (sig_n_prime256, sig_y,
			ten2k128[exp_y - exp_x - 20]);
  {
    res = ((sig_n_prime256.w[3] == 0) && (sig_n_prime256.w[2] == 0)
	   && (sig_n_prime256.w[1] == sig_x.w[1])
	   && (sig_n_prime256.w[0] == sig_x.w[0]));
    BID_RETURN (res);
  }

}
  //else{
  // recalculate y's significand upwards
__mul_64x128_to_192 (sig_n_prime192, ten2k64[exp_y - exp_x], sig_y);
{
  res = ((sig_n_prime192.w[2] == 0)
	 && (sig_n_prime192.w[1] == sig_x.w[1])
	 && (sig_n_prime192.w[0] == sig_x.w[0]));
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int, bid128_quiet_greater, x,
					  y)

     int res;
     int exp_x, exp_y;
     int diff;
     UINT128 sig_x, sig_y;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered, rather than 
  // equal : return 0
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
if ((x.w[1] & MASK_SNAN) == MASK_SNAN
    || (y.w[1] & MASK_SNAN) == MASK_SNAN) {
  *pfpsf |= INVALID_EXCEPTION;
}
{
  res = 0;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal (not Greater).
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 0;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  // if x is neg infinity, there is no way it is greater than y, return 0
  if (((x.w[1] & MASK_SIGN) == MASK_SIGN)) {
    res = 0;
    BID_RETURN (res);
  }
  // x is pos infinity, it is greater, unless y is positive infinity => 
  // return y!=pos_infinity
  else {
    res = (((y.w[1] & MASK_INF) != MASK_INF)
	   || ((y.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
} else if ((y.w[1] & MASK_INF) == MASK_INF) {
  // x is finite, so if y is positive infinity, then x is less, return 0
  //                 if y is negative infinity, then x is greater, return 1
  {
    res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //    If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //    If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}
  // if both numbers are zero, neither is greater => return NOTGREATERTHAN
if (x_is_zero && y_is_zero) {
  res = 0;
  BID_RETURN (res);
}
  // is x is zero, it is greater if Y is negative
else if (x_is_zero) {
  res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // is y is zero, X is greater if it is positive
else if (y_is_zero) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
  res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)
  // if exponents are the same, then we have a simple comparison 
  // of the significands
if (exp_y == exp_x) {
  res = (((sig_x.w[1] > sig_y.w[1])
	  || (sig_x.w[1] == sig_y.w[1]
	      && sig_x.w[0] >= sig_y.w[0])) ^ ((x.w[1] & MASK_SIGN) ==
					       MASK_SIGN));
  BID_RETURN (res);
}
  // if both components are either bigger or smaller, 
  // it is clear what needs to be done
if ((sig_x.w[1] > sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] > sig_y.w[0]))
    && exp_x >= exp_y) {
  {
    res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }
}
if ((sig_x.w[1] < sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] < sig_y.w[0]))
    && exp_x <= exp_y) {
  {
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }
}

diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
if (diff > 0) {	// to simplify the loop below,

  // if exp_x is 33 greater than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }	// difference cannot be greater than 10^33

  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);

    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_y.w[1]
	&& (sig_n_prime256.w[0] == sig_y.w[0])) {
      res = 0;
      BID_RETURN (res);
    }	// if equal, return 0
    {
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN));
      BID_RETURN (res);
    }
  }
  //else { //128 by 64 bit multiply -> 192 bits
  __mul_64x128_to_192 (sig_n_prime192, ten2k64[diff], sig_x);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
      && (sig_n_prime192.w[0] == sig_y.w[0])) {
    res = 0;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res = (((sig_n_prime192.w[2] > 0) ||
	    (sig_n_prime192.w[1] > sig_y.w[1]) ||
	    (sig_n_prime192.w[1] == sig_y.w[1] &&
	     sig_n_prime192.w[0] > sig_y.w[0])) ^
	   ((y.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}

diff = exp_y - exp_x;

  // if exp_x is 33 less than exp_y, no need for compensation
if (diff > 33) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
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
    res = 0;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res = ((sig_n_prime256.w[3] != 0 || sig_n_prime256.w[2] != 0 ||
	    (sig_n_prime256.w[1] > sig_x.w[1] ||
	     (sig_n_prime256.w[1] == sig_x.w[1] &&
	      sig_n_prime256.w[0] > sig_x.w[0]))) ^
	   ((x.w[1] & MASK_SIGN) != MASK_SIGN));
    BID_RETURN (res);
  }
}
  //else { //128 by 64 bit multiply -> 192 bits
  // adjust the y significand upwards
__mul_64x128_to_192 (sig_n_prime192, ten2k64[diff], sig_y);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
    && (sig_n_prime192.w[0] == sig_x.w[0])) {
  res = 0;
  BID_RETURN (res);
}	// if equal, return 0
{
  res = (sig_n_prime192.w[2] != 0
	 || (sig_n_prime192.w[1] > sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] >
		 sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int,
					  bid128_quiet_greater_equal, x,
					  y)

     int res;
     int exp_x, exp_y;
     int diff;
     UINT128 sig_x, sig_y;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered, 
  // rather than equal : return 1
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
if ((x.w[1] & MASK_SNAN) == MASK_SNAN
    || (y.w[1] & MASK_SNAN) == MASK_SNAN) {
  *pfpsf |= INVALID_EXCEPTION;
}
{
  res = 0;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal (not Greater).
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 1;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  // if x==neg_inf, { res = (y == neg_inf)?1:0; BID_RETURN (res) }
  if ((x.w[1] & MASK_SIGN) == MASK_SIGN)
    // x is -inf, so it is less than y unless y is -inf
  {
    res = (((y.w[1] & MASK_INF) == MASK_INF)
	   && (y.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  } else
    // x is pos_inf, no way for it to be less than y
  {
    res = 1;
    BID_RETURN (res);
  }
} else if ((y.w[1] & MASK_INF) == MASK_INF) {
  // x is finite, so if y is positive infinity, then x is less, return 0
  //                 if y is negative infinity, then x is greater, return 1
  {
    res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //    If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //    If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}
  // if both numbers are zero, neither is greater => return NOTGREATERTHAN
if (x_is_zero && y_is_zero) {
  res = 1;
  BID_RETURN (res);
}
  // is x is zero, it is greater if Y is negative
else if (x_is_zero) {
  res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // is y is zero, X is greater if it is positive
else if (y_is_zero) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
  res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)
  // if exponents are the same, then we have a simple comparison of the 
  // significands
if (exp_y == exp_x) {
  res = (((sig_x.w[1] > sig_y.w[1])
	  || (sig_x.w[1] == sig_y.w[1]
	      && sig_x.w[0] >= sig_y.w[0])) ^ ((x.w[1] & MASK_SIGN) ==
					       MASK_SIGN));
  BID_RETURN (res);
}
  // if both components are either bigger or smaller, 
  // it is clear what needs to be done
if (sig_x.w[1] >= sig_y.w[1] && sig_x.w[0] >= sig_y.w[0]
    && exp_x > exp_y) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
if (sig_x.w[1] <= sig_y.w[1] && sig_x.w[0] <= sig_y.w[0]
    && exp_x < exp_y) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}

diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
if (diff > 0) {	// to simplify the loop below,

  // if exp_x is 33 greater than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }	// difference cannot be greater than 10^33

  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);


    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_y.w[1]
	&& (sig_n_prime256.w[0] == sig_y.w[0])) {
      res = 1;
      BID_RETURN (res);
    }	// if equal, return 1
    {
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN));
      BID_RETURN (res);
    }
  }
  //else { //128 by 64 bit multiply -> 192 bits
  __mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_x);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
      && (sig_n_prime192.w[0] == sig_y.w[0])) {
    res = 1;
    BID_RETURN (res);
  }	// if equal, return 1
  {
    res = (((sig_n_prime192.w[2] > 0)
	    || (sig_n_prime192.w[1] > sig_y.w[1])
	    || (sig_n_prime192.w[1] == sig_y.w[1]
		&& sig_n_prime192.w[0] >
		sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}

diff = exp_y - exp_x;

  // if exp_x is 33 less than exp_y, no need for compensation
if (diff > 33) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
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
    res = 1;
    BID_RETURN (res);
  }	// if equal, return 1
  {
    res = ((sig_n_prime256.w[3] == 0 && sig_n_prime256.w[2] == 0
	    && (sig_n_prime256.w[1] < sig_x.w[1]
		|| (sig_n_prime256.w[1] == sig_x.w[1]
		    && sig_n_prime256.w[0] <
		    sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) ==
				     MASK_SIGN));
    BID_RETURN (res);
  }
}
  //else { //128 by 64 bit multiply -> 192 bits
  // adjust the y significand upwards
__mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_y);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
    && (sig_n_prime192.w[0] == sig_x.w[0])) {
  res = 1;
  BID_RETURN (res);
}	// if equal, return 1
{
  res = (sig_n_prime192.w[2] == 0
	 && (sig_n_prime192.w[1] < sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] <
		 sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int,
					  bid128_quiet_greater_unordered,
					  x, y)

     int res;
     int exp_x, exp_y;
     int diff;
     UINT128 sig_x, sig_y;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered, 
  // rather than 
  // equal : return 1
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
if ((x.w[1] & MASK_SNAN) == MASK_SNAN
    || (y.w[1] & MASK_SNAN) == MASK_SNAN) {
  *pfpsf |= INVALID_EXCEPTION;
}
{
  res = 1;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal (not Greater).
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 0;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  // if x is neg infinity, there is no way it is greater than y, return 0
  if (((x.w[1] & MASK_SIGN) == MASK_SIGN)) {
    res = 0;
    BID_RETURN (res);
  }
  // x is pos infinity, it is greater, unless y is positive infinity => 
  // return y!=pos_infinity
  else {
    res = (((y.w[1] & MASK_INF) != MASK_INF)
	   || ((y.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
} else if ((y.w[1] & MASK_INF) == MASK_INF) {
  // x is finite, so if y is positive infinity, then x is less, return 0
  //                 if y is negative infinity, then x is greater, return 1
  {
    res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //    If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //    If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}
  // if both numbers are zero, neither is greater => return NOTGREATERTHAN
if (x_is_zero && y_is_zero) {
  res = 0;
  BID_RETURN (res);
}
  // is x is zero, it is greater if Y is negative
else if (x_is_zero) {
  res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // is y is zero, X is greater if it is positive
else if (y_is_zero) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
  res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)
  // if exponents are the same, then we have a simple comparison of the 
  // significands
if (exp_y == exp_x) {
  res = (((sig_x.w[1] > sig_y.w[1])
	  || (sig_x.w[1] == sig_y.w[1]
	      && sig_x.w[0] >= sig_y.w[0])) ^ ((x.w[1] & MASK_SIGN) ==
					       MASK_SIGN));
  BID_RETURN (res);
}
  // if both components are either bigger or smaller, 
  // it is clear what needs to be done
if (sig_x.w[1] >= sig_y.w[1] && sig_x.w[0] >= sig_y.w[0]
    && exp_x > exp_y) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
if (sig_x.w[1] <= sig_y.w[1] && sig_x.w[0] <= sig_y.w[0]
    && exp_x < exp_y) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}

diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
if (diff > 0) {	// to simplify the loop below,

  // if exp_x is 33 greater than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }	// difference cannot be greater than 10^33

  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);


    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_y.w[1]
	&& (sig_n_prime256.w[0] == sig_y.w[0])) {
      res = 0;
      BID_RETURN (res);
    }	// if equal, return 0
    {
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN));
      BID_RETURN (res);
    }
  }
  //else { //128 by 64 bit multiply -> 192 bits
  __mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_x);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
      && (sig_n_prime192.w[0] == sig_y.w[0])) {
    res = 0;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res = (((sig_n_prime192.w[2] > 0)
	    || (sig_n_prime192.w[1] > sig_y.w[1])
	    || (sig_n_prime192.w[1] == sig_y.w[1]
		&& sig_n_prime192.w[0] >
		sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}

diff = exp_y - exp_x;

  // if exp_x is 33 less than exp_y, no need for compensation
if (diff > 33) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
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
    res = 0;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res = ((sig_n_prime256.w[3] == 0 && sig_n_prime256.w[2] == 0
	    && (sig_n_prime256.w[1] < sig_x.w[1]
		|| (sig_n_prime256.w[1] == sig_x.w[1]
		    && sig_n_prime256.w[0] <
		    sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) ==
				     MASK_SIGN));
    BID_RETURN (res);
  }
}
  //else { //128 by 64 bit multiply -> 192 bits
  // adjust the y significand upwards
__mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_y);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
    && (sig_n_prime192.w[0] == sig_x.w[0])) {
  res = 0;
  BID_RETURN (res);
}	// if equal, return 0
{
  res = (sig_n_prime192.w[2] == 0
	 && (sig_n_prime192.w[1] < sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] <
		 sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int, bid128_quiet_less, x, y)

     int res;
     int exp_x, exp_y;
     int diff;
     UINT128 sig_x, sig_y;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered, 
  // rather than equal : return 0
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
if ((x.w[1] & MASK_SNAN) == MASK_SNAN
    || (y.w[1] & MASK_SNAN) == MASK_SNAN) {
  *pfpsf |= INVALID_EXCEPTION;
}
{
  res = 0;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal.
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 0;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  // if x==neg_inf, { res = (y == neg_inf)?1:0; BID_RETURN (res) }
  if ((x.w[1] & MASK_SIGN) == MASK_SIGN)
    // x is -inf, so it is less than y unless y is -inf
  {
    res = (((y.w[1] & MASK_INF) != MASK_INF)
	   || (y.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  } else
    // x is pos_inf, no way for it to be less than y
  {
    res = 0;
    BID_RETURN (res);
  }
} else if ((y.w[1] & MASK_INF) == MASK_INF) {
  // x is finite, so if y is positive infinity, then x is less, return 0
  //                 if y is negative infinity, then x is greater, return 1
  {
    res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //    If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //    If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}
  // if both numbers are zero, neither is greater => return NOTGREATERTHAN
if (x_is_zero && y_is_zero) {
  res = 0;
  BID_RETURN (res);
}
  // is x is zero, it is greater if Y is negative
else if (x_is_zero) {
  res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // is y is zero, X is greater if it is positive
else if (y_is_zero) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
  res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)
  // if exponents are the same, then we have a simple comparison of the 
  // significands
if (exp_y == exp_x) {
  res = (((sig_x.w[1] > sig_y.w[1])
	  || (sig_x.w[1] == sig_y.w[1]
	      && sig_x.w[0] >= sig_y.w[0])) ^ ((x.w[1] & MASK_SIGN) !=
					       MASK_SIGN));
  BID_RETURN (res);
}
  // if both components are either bigger or smaller, 
  // it is clear what needs to be done
if ((sig_x.w[1] > sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] > sig_y.w[0]))
    && exp_x >= exp_y) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
if ((sig_x.w[1] < sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] < sig_y.w[0]))
    && exp_x <= exp_y) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}

diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
if (diff > 0) {	// to simplify the loop below,

  // if exp_x is 33 greater than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }	// difference cannot be greater than 10^33

  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);


    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_y.w[1]
	&& (sig_n_prime256.w[0] == sig_y.w[0])) {
      res = 0;
      BID_RETURN (res);
    }	// if equal, return 0
    {
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN));
      BID_RETURN (res);
    }
  }
  //else { //128 by 64 bit multiply -> 192 bits
  __mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_x);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
      && (sig_n_prime192.w[0] == sig_y.w[0])) {
    res = 0;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res = (((sig_n_prime192.w[2] > 0)
	    || (sig_n_prime192.w[1] > sig_y.w[1])
	    || (sig_n_prime192.w[1] == sig_y.w[1]
		&& sig_n_prime192.w[0] >
		sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN));
    BID_RETURN (res);
  }
}

diff = exp_y - exp_x;

  // if exp_x is 33 less than exp_y, no need for compensation
if (diff > 33) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
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
    res = 0;
    BID_RETURN (res);
  }	// if equal, return 1
  {
    res = ((sig_n_prime256.w[3] != 0 || sig_n_prime256.w[2] != 0
	    || (sig_n_prime256.w[1] > sig_x.w[1]
		|| (sig_n_prime256.w[1] == sig_x.w[1]
		    && sig_n_prime256.w[0] >
		    sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) ==
				     MASK_SIGN));
    BID_RETURN (res);
  }
}
  //else { //128 by 64 bit multiply -> 192 bits
  // adjust the y significand upwards
__mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_y);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
    && (sig_n_prime192.w[0] == sig_x.w[0])) {
  res = 0;
  BID_RETURN (res);
}	// if equal, return 0
{
  res = (sig_n_prime192.w[2] != 0
	 || (sig_n_prime192.w[1] > sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] >
		 sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int, bid128_quiet_less_equal,
					  x, y)

     int res;
     int exp_x, exp_y;
     int diff;
     UINT128 sig_x, sig_y;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered, 
  // rather than equal : return 0
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
if ((x.w[1] & MASK_SNAN) == MASK_SNAN
    || (y.w[1] & MASK_SNAN) == MASK_SNAN) {
  *pfpsf |= INVALID_EXCEPTION;
}
{
  res = 0;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal (not Greater).
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 1;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  // if x is neg infinity, there is no way it is greater than y, return 1
  if (((x.w[1] & MASK_SIGN) == MASK_SIGN)) {
    res = 1;
    BID_RETURN (res);
  }
  // x is pos infinity, it is greater, unless y is positive infinity => 
  // return y!=pos_infinity
  else {
    res = (((y.w[1] & MASK_INF) == MASK_INF)
	   && ((y.w[1] & MASK_SIGN) != MASK_SIGN));
    BID_RETURN (res);
  }
} else if ((y.w[1] & MASK_INF) == MASK_INF) {
  // x is finite, so if y is positive infinity, then x is less, return 0
  //                 if y is negative infinity, then x is greater, return 1
  {
    res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //    If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //    If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}
  // if both numbers are zero, neither is greater => return NOTGREATERTHAN
if (x_is_zero && y_is_zero) {
  res = 1;
  BID_RETURN (res);
}
  // is x is zero, it is greater if Y is negative
else if (x_is_zero) {
  res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // is y is zero, X is greater if it is positive
else if (y_is_zero) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
  res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)
  // if exponents are the same, then we have a simple comparison of the 
  // significands
if (exp_y == exp_x) {
  res = (((sig_x.w[1] > sig_y.w[1]) || (sig_x.w[1] == sig_y.w[1] &&
					sig_x.w[0] >=
					sig_y.w[0])) ^ ((x.
							 w[1] &
							 MASK_SIGN) !=
							MASK_SIGN));
  BID_RETURN (res);
}
  // if both components are either bigger or smaller, 
  // it is clear what needs to be done
if ((sig_x.w[1] > sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] > sig_y.w[0]))
    && exp_x >= exp_y) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
if ((sig_x.w[1] < sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] < sig_y.w[0]))
    && exp_x <= exp_y) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}

diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
if (diff > 0) {	// to simplify the loop below,

  // if exp_x is 33 greater than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }	// difference cannot be greater than 10^33

  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);


    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_y.w[1]
	&& (sig_n_prime256.w[0] == sig_y.w[0])) {
      res = 1;
      BID_RETURN (res);
    }	// if equal, return 0
    {
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN));
      BID_RETURN (res);
    }
  }
  //else { //128 by 64 bit multiply -> 192 bits
  __mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_x);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
      && (sig_n_prime192.w[0] == sig_y.w[0])) {
    res = 1;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res = (((sig_n_prime192.w[2] > 0)
	    || (sig_n_prime192.w[1] > sig_y.w[1])
	    || (sig_n_prime192.w[1] == sig_y.w[1]
		&& sig_n_prime192.w[0] >
		sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN));
    BID_RETURN (res);
  }
}

diff = exp_y - exp_x;

  // if exp_x is 33 less than exp_y, no need for compensation
if (diff > 33) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
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
    res = 1;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res =
      ((sig_n_prime256.w[3] != 0 || sig_n_prime256.w[2] != 0
	|| (sig_n_prime256.w[1] > sig_x.w[1]
	    || (sig_n_prime256.w[1] == sig_x.w[1]
		&& sig_n_prime256.w[0] >
		sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}
  //else { //128 by 64 bit multiply -> 192 bits
  // adjust the y significand upwards
__mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_y);

  // if postitive, return whichever significand is larger 
      // (converse if negative)
if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
    && (sig_n_prime192.w[0] == sig_x.w[0])) {
  res = 1;
  BID_RETURN (res);
}	// if equal, return 0
{
  res = (sig_n_prime192.w[2] != 0
	 || (sig_n_prime192.w[1] > sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] >
		 sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int,
					  bid128_quiet_less_unordered,
					  x, y)

     int res;
     int exp_x, exp_y;
     int diff;
     UINT128 sig_x, sig_y;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
if ((x.w[1] & MASK_SNAN) == MASK_SNAN
    || (y.w[1] & MASK_SNAN) == MASK_SNAN) {
  *pfpsf |= INVALID_EXCEPTION;
}
{
  res = 1;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal.
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 0;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  // if x==neg_inf, { res = (y == neg_inf)?1:0; BID_RETURN (res) }
  if ((x.w[1] & MASK_SIGN) == MASK_SIGN)
    // x is -inf, so it is less than y unless y is -inf
  {
    res = (((y.w[1] & MASK_INF) != MASK_INF)
	   || (y.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  } else
    // x is pos_inf, no way for it to be less than y
  {
    res = 0;
    BID_RETURN (res);
  }
} else if ((y.w[1] & MASK_INF) == MASK_INF) {
  // x is finite, so if y is positive infinity, then x is less, return 0
  //                 if y is negative infinity, then x is greater, return 1
  {
    res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //    If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //    If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}
  // if both numbers are zero, neither is greater => return NOTGREATERTHAN
if (x_is_zero && y_is_zero) {
  res = 0;
  BID_RETURN (res);
}
  // is x is zero, it is greater if Y is negative
else if (x_is_zero) {
  res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // is y is zero, X is greater if it is positive
else if (y_is_zero) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
  res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)
  // if exponents are the same, then we have a simple comparison 
  // of the significands
if (exp_y == exp_x) {
  res = (((sig_x.w[1] > sig_y.w[1])
	  || (sig_x.w[1] == sig_y.w[1]
	      && sig_x.w[0] >= sig_y.w[0])) ^ ((x.w[1] & MASK_SIGN) !=
					       MASK_SIGN));
  BID_RETURN (res);
}
  // if both components are either bigger or smaller, 
  // it is clear what needs to be done
if ((sig_x.w[1] > sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] > sig_y.w[0]))
    && exp_x >= exp_y) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
if ((sig_x.w[1] < sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] < sig_y.w[0]))
    && exp_x <= exp_y) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}

diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
if (diff > 0) {	// to simplify the loop below,

  // if exp_x is 33 greater than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }	// difference cannot be greater than 10^33

  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);


    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_y.w[1]
	&& (sig_n_prime256.w[0] == sig_y.w[0])) {
      res = 0;
      BID_RETURN (res);
    }	// if equal, return 0
    {
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN));
      BID_RETURN (res);
    }
  }
  //else { //128 by 64 bit multiply -> 192 bits
  __mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_x);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
      && (sig_n_prime192.w[0] == sig_y.w[0])) {
    res = 0;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res = (((sig_n_prime192.w[2] > 0)
	    || (sig_n_prime192.w[1] > sig_y.w[1])
	    || (sig_n_prime192.w[1] == sig_y.w[1]
		&& sig_n_prime192.w[0] >
		sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN));
    BID_RETURN (res);
  }
}

diff = exp_y - exp_x;

  // if exp_x is 33 less than exp_y, no need for compensation
if (diff > 33) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
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
    res = 0;
    BID_RETURN (res);
  }	// if equal, return 1
  {
    res =
      ((sig_n_prime256.w[3] != 0 || sig_n_prime256.w[2] != 0
	|| (sig_n_prime256.w[1] > sig_x.w[1]
	    || (sig_n_prime256.w[1] == sig_x.w[1]
		&& sig_n_prime256.w[0] >
		sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}
  //else { //128 by 64 bit multiply -> 192 bits
  // adjust the y significand upwards
__mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_y);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
    && (sig_n_prime192.w[0] == sig_x.w[0])) {
  res = 0;
  BID_RETURN (res);
}	// if equal, return 0
{
  res = (sig_n_prime192.w[2] != 0
	 || (sig_n_prime192.w[1] > sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] >
		 sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int, bid128_quiet_not_equal,
					  x, y)

     int res;
     int exp_x, exp_y, exp_t;
     UINT128 sig_x, sig_y, sig_t;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered, 
  // rather than equal : return 0
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
if ((x.w[1] & MASK_SNAN) == MASK_SNAN
    || (y.w[1] & MASK_SNAN) == MASK_SNAN) {
  *pfpsf |= INVALID_EXCEPTION;
}
{
  res = 1;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equivalent.
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 0;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  if ((y.w[1] & MASK_INF) == MASK_INF) {
    res = (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  } else {
    res = 1;
    BID_RETURN (res);
  }
}
if ((y.w[1] & MASK_INF) == MASK_INF) {
  res = 1;
  BID_RETURN (res);
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //    If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //    If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}

if (x_is_zero && y_is_zero) {
  res = 0;
  BID_RETURN (res);
} else if ((x_is_zero && !y_is_zero) || (!x_is_zero && y_is_zero)) {
  res = 1;
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ => not equal : return 0
if ((x.w[1] ^ y.w[1]) & MASK_SIGN) {
  res = 1;
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)
if (exp_x > exp_y) {	// to simplify the loop below,
  SWAP (exp_x, exp_y, exp_t);	// put the larger exp in y,
  SWAP (sig_x.w[1], sig_y.w[1], sig_t.w[1]);	// and the smaller exp in x
  SWAP (sig_x.w[0], sig_y.w[0], sig_t.w[0]);	// and the smaller exp in x
}


if (exp_y - exp_x > 33) {
  res = 1;
  BID_RETURN (res);
}	// difference cannot be greater than 10^33

if (exp_y - exp_x > 19) {
  // recalculate y's significand upwards
  __mul_128x128_to_256 (sig_n_prime256, sig_y,
			ten2k128[exp_y - exp_x - 20]);
  {
    res = ((sig_n_prime256.w[3] != 0) || (sig_n_prime256.w[2] != 0)
	   || (sig_n_prime256.w[1] != sig_x.w[1])
	   || (sig_n_prime256.w[0] != sig_x.w[0]));
    BID_RETURN (res);
  }

}
  //else{
  // recalculate y's significand upwards
__mul_64x128_to192 (sig_n_prime192, ten2k64[exp_y - exp_x], sig_y);
{
  res = ((sig_n_prime192.w[2] != 0)
	 || (sig_n_prime192.w[1] != sig_x.w[1])
	 || (sig_n_prime192.w[0] != sig_x.w[0]));
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int, bid128_quiet_not_greater,
					  x, y)

     int res;
     int exp_x, exp_y;
     int diff;
     UINT128 sig_x, sig_y;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered, 
  // rather than equal : return 0
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
if ((x.w[1] & MASK_SNAN) == MASK_SNAN
    || (y.w[1] & MASK_SNAN) == MASK_SNAN) {
  *pfpsf |= INVALID_EXCEPTION;
}
{
  res = 1;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal (not Greater).
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 1;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  // if x is neg infinity, there is no way it is greater than y, return 1
  if (((x.w[1] & MASK_SIGN) == MASK_SIGN)) {
    res = 1;
    BID_RETURN (res);
  }
  // x is pos infinity, it is greater, unless y is positive infinity => return y!=pos_infinity
  else {
    res = (((y.w[1] & MASK_INF) == MASK_INF)
	   && ((y.w[1] & MASK_SIGN) != MASK_SIGN));
    BID_RETURN (res);
  }
} else if ((y.w[1] & MASK_INF) == MASK_INF) {
  // x is finite, so if y is positive infinity, then x is less, return 0
  //                 if y is negative infinity, then x is greater, return 1
  {
    res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //    If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //    If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}
  // if both numbers are zero, neither is greater => return NOTGREATERTHAN
if (x_is_zero && y_is_zero) {
  res = 1;
  BID_RETURN (res);
}
  // is x is zero, it is greater if Y is negative
else if (x_is_zero) {
  res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // is y is zero, X is greater if it is positive
else if (y_is_zero) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
  res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)
  // if exponents are the same, then we have a simple comparison 
  // of the significands
if (exp_y == exp_x) {
  res = (((sig_x.w[1] > sig_y.w[1])
	  || (sig_x.w[1] == sig_y.w[1]
	      && sig_x.w[0] >= sig_y.w[0])) ^ ((x.w[1] & MASK_SIGN) !=
					       MASK_SIGN));
  BID_RETURN (res);
}
  // if both components are either bigger or smaller, 
  // it is clear what needs to be done
if ((sig_x.w[1] > sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] > sig_y.w[0]))
    && exp_x >= exp_y) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
if ((sig_x.w[1] < sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] < sig_y.w[0]))
    && exp_x <= exp_y) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}

diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
if (diff > 0) {	// to simplify the loop below,

  // if exp_x is 33 greater than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }	// difference cannot be greater than 10^33

  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);


    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_y.w[1]
	&& (sig_n_prime256.w[0] == sig_y.w[0])) {
      res = 1;
      BID_RETURN (res);
    }	// if equal, return 0
    {
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN));
      BID_RETURN (res);
    }
  }
  //else { //128 by 64 bit multiply -> 192 bits
  __mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_x);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
      && (sig_n_prime192.w[0] == sig_y.w[0])) {
    res = 1;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res = (((sig_n_prime192.w[2] > 0)
	    || (sig_n_prime192.w[1] > sig_y.w[1])
	    || (sig_n_prime192.w[1] == sig_y.w[1]
		&& sig_n_prime192.w[0] >
		sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN));
    BID_RETURN (res);
  }
}

diff = exp_y - exp_x;

  // if exp_x is 33 less than exp_y, no need for compensation
if (diff > 33) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
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
    res = 1;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res =
      ((sig_n_prime256.w[3] != 0 || sig_n_prime256.w[2] != 0
	|| (sig_n_prime256.w[1] > sig_x.w[1]
	    || (sig_n_prime256.w[1] == sig_x.w[1]
		&& sig_n_prime256.w[0] >
		sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}
  //else { //128 by 64 bit multiply -> 192 bits
  // adjust the y significand upwards
__mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_y);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
    && (sig_n_prime192.w[0] == sig_x.w[0])) {
  res = 1;
  BID_RETURN (res);
}	// if equal, return 0
{
  res = (sig_n_prime192.w[2] != 0
	 || (sig_n_prime192.w[1] > sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] >
		 sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int, bid128_quiet_not_less, x,
					  y)

     int res;
     int exp_x, exp_y;
     int diff;
     UINT128 sig_x, sig_y;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered, 
  // rather than equal : return 1
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
if ((x.w[1] & MASK_SNAN) == MASK_SNAN
    || (y.w[1] & MASK_SNAN) == MASK_SNAN) {
  *pfpsf |= INVALID_EXCEPTION;
}
{
  res = 1;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal (not Greater).
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 1;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  // if x==neg_inf, { res = (y == neg_inf)?1:0; BID_RETURN (res) }
  if ((x.w[1] & MASK_SIGN) == MASK_SIGN)
    // x is -inf, so it is less than y unless y is -inf
  {
    res = (((y.w[1] & MASK_INF) == MASK_INF)
	   && (y.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  } else
    // x is pos_inf, no way for it to be less than y
  {
    res = 1;
    BID_RETURN (res);
  }
} else if ((y.w[1] & MASK_INF) == MASK_INF) {
  // x is finite, so if y is positive infinity, then x is less, return 0
  //                 if y is negative infinity, then x is greater, return 1
  {
    res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}
  // if both numbers are zero, neither is greater => return NOTGREATERTHAN
if (x_is_zero && y_is_zero) {
  res = 1;
  BID_RETURN (res);
}
  // is x is zero, it is greater if Y is negative
else if (x_is_zero) {
  res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // is y is zero, X is greater if it is positive
else if (y_is_zero) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
  res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)

  // if exponents are the same, then we have a simple comparison 
  // of the significands
if (exp_y == exp_x) {
  res = (((sig_x.w[1] > sig_y.w[1])
	  || (sig_x.w[1] == sig_y.w[1]
	      && sig_x.w[0] >= sig_y.w[0])) ^ ((x.w[1] & MASK_SIGN) ==
					       MASK_SIGN));
  BID_RETURN (res);
}
  // if both components are either bigger or smaller, 
  // it is clear what needs to be done
if (sig_x.w[1] >= sig_y.w[1] && sig_x.w[0] >= sig_y.w[0]
    && exp_x > exp_y) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
if (sig_x.w[1] <= sig_y.w[1] && sig_x.w[0] <= sig_y.w[0]
    && exp_x < exp_y) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}

diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
if (diff > 0) {	// to simplify the loop below,

  // if exp_x is 33 greater than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }	// difference cannot be greater than 10^33

  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);


    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_y.w[1]
	&& (sig_n_prime256.w[0] == sig_y.w[0])) {
      res = 1;
      BID_RETURN (res);
    }	// if equal, return 1
    {
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN));
      BID_RETURN (res);
    }
  }
  //else { //128 by 64 bit multiply -> 192 bits
  __mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_x);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
      && (sig_n_prime192.w[0] == sig_y.w[0])) {
    res = 1;
    BID_RETURN (res);
  }	// if equal, return 1
  {
    res = (((sig_n_prime192.w[2] > 0)
	    || (sig_n_prime192.w[1] > sig_y.w[1])
	    || (sig_n_prime192.w[1] == sig_y.w[1]
		&& sig_n_prime192.w[0] >
		sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}

diff = exp_y - exp_x;

  // if exp_x is 33 less than exp_y, no need for compensation
if (diff > 33) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
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
    res = 1;
    BID_RETURN (res);
  }	// if equal, return 1
  {
    res =
      ((sig_n_prime256.w[3] == 0 && sig_n_prime256.w[2] == 0
	&& (sig_n_prime256.w[1] < sig_x.w[1]
	    || (sig_n_prime256.w[1] == sig_x.w[1]
		&& sig_n_prime256.w[0] <
		sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}
  //else { //128 by 64 bit multiply -> 192 bits
  // adjust the y significand upwards
__mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_y);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
    && (sig_n_prime192.w[0] == sig_x.w[0])) {
  res = 1;
  BID_RETURN (res);
}	// if equal, return 1
{
  res = (sig_n_prime192.w[2] == 0
	 && (sig_n_prime192.w[1] < sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] <
		 sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int, bid128_quiet_ordered, x,
					  y)

     int res;

  // NaN (CASE1)
  // if either number is NAN, the comparison is ordered : return 1
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
if ((x.w[1] & MASK_SNAN) == MASK_SNAN
    || (y.w[1] & MASK_SNAN) == MASK_SNAN) {
  *pfpsf |= INVALID_EXCEPTION;
}
{
  res = 0;
  BID_RETURN (res);
}
}
{
  res = 1;
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int, bid128_quiet_unordered,
					  x, y)

     int res;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered : return 1
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
if ((x.w[1] & MASK_SNAN) == MASK_SNAN
    || (y.w[1] & MASK_SNAN) == MASK_SNAN) {
  *pfpsf |= INVALID_EXCEPTION;
}
{
  res = 1;
  BID_RETURN (res);
}
}
{
  res = 0;
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int, bid128_signaling_greater,
					  x, y)

     int res;
     int exp_x, exp_y;
     int diff;
     UINT128 sig_x, sig_y;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered, 
  // rather than equal : return 0
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
*pfpsf |= INVALID_EXCEPTION;
{
  res = 0;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal (not Greater).
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 0;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  // if x is neg infinity, there is no way it is greater than y, return 0
  if (((x.w[1] & MASK_SIGN) == MASK_SIGN)) {
    res = 0;
    BID_RETURN (res);
  }
  // x is pos infinity, it is greater, unless y is positive infinity => return y!=pos_infinity
  else {
    res = (((y.w[1] & MASK_INF) != MASK_INF)
	   || ((y.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
} else if ((y.w[1] & MASK_INF) == MASK_INF) {
  // x is finite, so if y is positive infinity, then x is less, return 0
  //                 if y is negative infinity, then x is greater, return 1
  {
    res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}
  // if both numbers are zero, neither is greater => return NOTGREATERTHAN
if (x_is_zero && y_is_zero) {
  res = 0;
  BID_RETURN (res);
}
  // is x is zero, it is greater if Y is negative
else if (x_is_zero) {
  res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // is y is zero, X is greater if it is positive
else if (y_is_zero) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
  res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)
  // if exponents are the same, then we have a simple comparison 
  // of the significands
if (exp_y == exp_x) {
  res = (((sig_x.w[1] > sig_y.w[1])
	  || (sig_x.w[1] == sig_y.w[1]
	      && sig_x.w[0] >= sig_y.w[0])) ^ ((x.w[1] & MASK_SIGN) ==
					       MASK_SIGN));
  BID_RETURN (res);
}
  // if both components are either bigger or smaller, 
  // it is clear what needs to be done
if ((sig_x.w[1] > sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] > sig_y.w[0]))
    && exp_x >= exp_y) {
  {
    res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }
}
if ((sig_x.w[1] < sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] < sig_y.w[0]))
    && exp_x <= exp_y) {
  {
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }
}

diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
if (diff > 0) {	// to simplify the loop below,

  // if exp_x is 33 greater than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }	// difference cannot be greater than 10^33

  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);

    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_y.w[1]
	&& (sig_n_prime256.w[0] == sig_y.w[0])) {
      res = 0;
      BID_RETURN (res);
    }	// if equal, return 0
    {
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN));
      BID_RETURN (res);
    }
  }
  //else { //128 by 64 bit multiply -> 192 bits
  __mul_64x128_to_192 (sig_n_prime192, ten2k64[diff], sig_x);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
      && (sig_n_prime192.w[0] == sig_y.w[0])) {
    res = 0;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res = (((sig_n_prime192.w[2] > 0)
	    || (sig_n_prime192.w[1] > sig_y.w[1])
	    || (sig_n_prime192.w[1] == sig_y.w[1]
		&& sig_n_prime192.w[0] >
		sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}

diff = exp_y - exp_x;

  // if exp_x is 33 less than exp_y, no need for compensation
if (diff > 33) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
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
    res = 0;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res =
      ((sig_n_prime256.w[3] != 0 || sig_n_prime256.w[2] != 0
	|| (sig_n_prime256.w[1] > sig_x.w[1]
	    || (sig_n_prime256.w[1] == sig_x.w[1]
		&& sig_n_prime256.w[0] >
		sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) != MASK_SIGN));
    BID_RETURN (res);
  }
}
  //else { //128 by 64 bit multiply -> 192 bits
  // adjust the y significand upwards
__mul_64x128_to_192 (sig_n_prime192, ten2k64[diff], sig_y);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
    && (sig_n_prime192.w[0] == sig_x.w[0])) {
  res = 0;
  BID_RETURN (res);
}	// if equal, return 0
{
  res = (sig_n_prime192.w[2] != 0
	 || (sig_n_prime192.w[1] > sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] >
		 sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int,
					  bid128_signaling_greater_equal,
					  x, y)

     int res;
     int exp_x, exp_y;
     int diff;
     UINT128 sig_x, sig_y;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered, 
  // rather than equal : return 1
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
*pfpsf |= INVALID_EXCEPTION;
{
  res = 0;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal (not Greater).
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 1;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  // if x==neg_inf, { res = (y == neg_inf)?1:0; BID_RETURN (res) }
  if ((x.w[1] & MASK_SIGN) == MASK_SIGN)
    // x is -inf, so it is less than y unless y is -inf
  {
    res = (((y.w[1] & MASK_INF) == MASK_INF)
	   && (y.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  } else
    // x is pos_inf, no way for it to be less than y
  {
    res = 1;
    BID_RETURN (res);
  }
} else if ((y.w[1] & MASK_INF) == MASK_INF) {
  // x is finite, so if y is positive infinity, then x is less, return 0
  //                 if y is negative infinity, then x is greater, return 1
  {
    res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}
  // if both numbers are zero, neither is greater => return NOTGREATERTHAN
if (x_is_zero && y_is_zero) {
  res = 1;
  BID_RETURN (res);
}
  // is x is zero, it is greater if Y is negative
else if (x_is_zero) {
  res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // is y is zero, X is greater if it is positive
else if (y_is_zero) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
  res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)
  // if exponents are the same, then we have a simple comparison 
  // of the significands
if (exp_y == exp_x) {
  res = (((sig_x.w[1] > sig_y.w[1])
	  || (sig_x.w[1] == sig_y.w[1]
	      && sig_x.w[0] >= sig_y.w[0])) ^ ((x.w[1] & MASK_SIGN) ==
					       MASK_SIGN));
  BID_RETURN (res);
}
  // if both components are either bigger or smaller, 
  // it is clear what needs to be done
if (sig_x.w[1] >= sig_y.w[1] && sig_x.w[0] >= sig_y.w[0]
    && exp_x > exp_y) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
if (sig_x.w[1] <= sig_y.w[1] && sig_x.w[0] <= sig_y.w[0]
    && exp_x < exp_y) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}

diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
if (diff > 0) {	// to simplify the loop below,

  // if exp_x is 33 greater than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }	// difference cannot be greater than 10^33

  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);


    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_y.w[1]
	&& (sig_n_prime256.w[0] == sig_y.w[0])) {
      res = 1;
      BID_RETURN (res);
    }	// if equal, return 1
    {
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN));
      BID_RETURN (res);
    }
  }
  //else { //128 by 64 bit multiply -> 192 bits
  __mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_x);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
      && (sig_n_prime192.w[0] == sig_y.w[0])) {
    res = 1;
    BID_RETURN (res);
  }	// if equal, return 1
  {
    res = (((sig_n_prime192.w[2] > 0)
	    || (sig_n_prime192.w[1] > sig_y.w[1])
	    || (sig_n_prime192.w[1] == sig_y.w[1]
		&& sig_n_prime192.w[0] >
		sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}

diff = exp_y - exp_x;

  // if exp_x is 33 less than exp_y, no need for compensation
if (diff > 33) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
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
    res = 1;
    BID_RETURN (res);
  }	// if equal, return 1
  {
    res =
      ((sig_n_prime256.w[3] == 0 && sig_n_prime256.w[2] == 0
	&& (sig_n_prime256.w[1] < sig_x.w[1]
	    || (sig_n_prime256.w[1] == sig_x.w[1]
		&& sig_n_prime256.w[0] <
		sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}
  //else { //128 by 64 bit multiply -> 192 bits
  // adjust the y significand upwards
__mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_y);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
    && (sig_n_prime192.w[0] == sig_x.w[0])) {
  res = 1;
  BID_RETURN (res);
}	// if equal, return 1
{
  res = (sig_n_prime192.w[2] == 0
	 && (sig_n_prime192.w[1] < sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] <
		 sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int,
					  bid128_signaling_greater_unordered,
					  x, y)

     int res;
     int exp_x, exp_y;
     int diff;
     UINT128 sig_x, sig_y;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered, 
  // rather than equal : return 1
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
*pfpsf |= INVALID_EXCEPTION;
{
  res = 1;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal (not Greater).
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 0;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  // if x is neg infinity, there is no way it is greater than y, return 0
  if (((x.w[1] & MASK_SIGN) == MASK_SIGN)) {
    res = 0;
    BID_RETURN (res);
  }
  // x is pos infinity, it is greater, unless y is positive infinity => return y!=pos_infinity
  else {
    res = (((y.w[1] & MASK_INF) != MASK_INF)
	   || ((y.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
} else if ((y.w[1] & MASK_INF) == MASK_INF) {
  // x is finite, so if y is positive infinity, then x is less, return 0
  //                 if y is negative infinity, then x is greater, return 1
  {
    res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}
  // if both numbers are zero, neither is greater => return NOTGREATERTHAN
if (x_is_zero && y_is_zero) {
  res = 0;
  BID_RETURN (res);
}
  // is x is zero, it is greater if Y is negative
else if (x_is_zero) {
  res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // is y is zero, X is greater if it is positive
else if (y_is_zero) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
  res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)
  // if exponents are the same, then we have a simple comparison 
  // of the significands
if (exp_y == exp_x) {
  res = (((sig_x.w[1] > sig_y.w[1])
	  || (sig_x.w[1] == sig_y.w[1]
	      && sig_x.w[0] >= sig_y.w[0])) ^ ((x.w[1] & MASK_SIGN) ==
					       MASK_SIGN));
  BID_RETURN (res);
}
  // if both components are either bigger or smaller, 
  // it is clear what needs to be done
if (sig_x.w[1] >= sig_y.w[1] && sig_x.w[0] >= sig_y.w[0]
    && exp_x > exp_y) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
if (sig_x.w[1] <= sig_y.w[1] && sig_x.w[0] <= sig_y.w[0]
    && exp_x < exp_y) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}

diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
if (diff > 0) {	// to simplify the loop below,

  // if exp_x is 33 greater than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }	// difference cannot be greater than 10^33

  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);


    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_y.w[1]
	&& (sig_n_prime256.w[0] == sig_y.w[0])) {
      res = 0;
      BID_RETURN (res);
    }	// if equal, return 0
    {
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN));
      BID_RETURN (res);
    }
  }
  //else { //128 by 64 bit multiply -> 192 bits
  __mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_x);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
      && (sig_n_prime192.w[0] == sig_y.w[0])) {
    res = 0;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res = (((sig_n_prime192.w[2] > 0)
	    || (sig_n_prime192.w[1] > sig_y.w[1])
	    || (sig_n_prime192.w[1] == sig_y.w[1]
		&& sig_n_prime192.w[0] >
		sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}

diff = exp_y - exp_x;

  // if exp_x is 33 less than exp_y, no need for compensation
if (diff > 33) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
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
    res = 0;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res =
      ((sig_n_prime256.w[3] == 0 && sig_n_prime256.w[2] == 0
	&& (sig_n_prime256.w[1] < sig_x.w[1]
	    || (sig_n_prime256.w[1] == sig_x.w[1]
		&& sig_n_prime256.w[0] <
		sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}
  //else { //128 by 64 bit multiply -> 192 bits
  // adjust the y significand upwards
__mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_y);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
    && (sig_n_prime192.w[0] == sig_x.w[0])) {
  res = 0;
  BID_RETURN (res);
}	// if equal, return 0
{
  res = (sig_n_prime192.w[2] == 0
	 && (sig_n_prime192.w[1] < sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] <
		 sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int, bid128_signaling_less, x,
					  y)

     int res;
     int exp_x, exp_y;
     int diff;
     UINT128 sig_x, sig_y;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered, 
  // rather than equal : return 0
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
*pfpsf |= INVALID_EXCEPTION;
{
  res = 0;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal.
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 0;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  // if x==neg_inf, { res = (y == neg_inf)?1:0; BID_RETURN (res) }
  if ((x.w[1] & MASK_SIGN) == MASK_SIGN)
    // x is -inf, so it is less than y unless y is -inf
  {
    res = (((y.w[1] & MASK_INF) != MASK_INF)
	   || (y.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  } else
    // x is pos_inf, no way for it to be less than y
  {
    res = 0;
    BID_RETURN (res);
  }
} else if ((y.w[1] & MASK_INF) == MASK_INF) {
  // x is finite, so if y is positive infinity, then x is less, return 0
  //                 if y is negative infinity, then x is greater, return 1
  {
    res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}
  // if both numbers are zero, neither is greater => return NOTGREATERTHAN
if (x_is_zero && y_is_zero) {
  res = 0;
  BID_RETURN (res);
}
  // is x is zero, it is greater if Y is negative
else if (x_is_zero) {
  res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // is y is zero, X is greater if it is positive
else if (y_is_zero) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
  res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)
  // if exponents are the same, then we have a simple comparison 
  // of the significands
if (exp_y == exp_x) {
  res = (((sig_x.w[1] > sig_y.w[1])
	  || (sig_x.w[1] == sig_y.w[1]
	      && sig_x.w[0] >= sig_y.w[0])) ^ ((x.w[1] & MASK_SIGN) !=
					       MASK_SIGN));
  BID_RETURN (res);
}
  // if both components are either bigger or smaller, 
  // it is clear what needs to be done
if ((sig_x.w[1] > sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] > sig_y.w[0]))
    && exp_x >= exp_y) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
if ((sig_x.w[1] < sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] < sig_y.w[0]))
    && exp_x <= exp_y) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}

diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
if (diff > 0) {	// to simplify the loop below,

  // if exp_x is 33 greater than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }	// difference cannot be greater than 10^33

  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);


    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_y.w[1]
	&& (sig_n_prime256.w[0] == sig_y.w[0])) {
      res = 0;
      BID_RETURN (res);
    }	// if equal, return 0
    {
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN));
      BID_RETURN (res);
    }
  }
  //else { //128 by 64 bit multiply -> 192 bits
  __mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_x);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
      && (sig_n_prime192.w[0] == sig_y.w[0])) {
    res = 0;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res = (((sig_n_prime192.w[2] > 0)
	    || (sig_n_prime192.w[1] > sig_y.w[1])
	    || (sig_n_prime192.w[1] == sig_y.w[1]
		&& sig_n_prime192.w[0] >
		sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN));
    BID_RETURN (res);
  }
}

diff = exp_y - exp_x;

  // if exp_x is 33 less than exp_y, |x| < |y|, return 1 if positive
if (diff > 33) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
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
    res = 0;
    BID_RETURN (res);
  }	// if equal, return 1
  {
    res =
      ((sig_n_prime256.w[3] != 0 || sig_n_prime256.w[2] != 0
	|| (sig_n_prime256.w[1] > sig_x.w[1]
	    || (sig_n_prime256.w[1] == sig_x.w[1]
		&& sig_n_prime256.w[0] >
		sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}
  //else { //128 by 64 bit multiply -> 192 bits
  // adjust the y significand upwards
__mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_y);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
    && (sig_n_prime192.w[0] == sig_x.w[0])) {
  res = 0;
  BID_RETURN (res);
}	// if equal, return 0
{
  res = (sig_n_prime192.w[2] != 0
	 || (sig_n_prime192.w[1] > sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] >
		 sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int,
					  bid128_signaling_less_equal,
					  x, y)

     int res;
     int exp_x, exp_y;
     int diff;
     UINT128 sig_x, sig_y;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered, 
  // rather than equal : return 0
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
*pfpsf |= INVALID_EXCEPTION;
{
  res = 0;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal (not Greater).
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 1;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  // if x is neg infinity, there is no way it is greater than y, return 1
  if (((x.w[1] & MASK_SIGN) == MASK_SIGN)) {
    res = 1;
    BID_RETURN (res);
  }
  // x is pos infinity, it is greater, unless y is positive infinity => return y!=pos_infinity
  else {
    res = (((y.w[1] & MASK_INF) == MASK_INF)
	   && ((y.w[1] & MASK_SIGN) != MASK_SIGN));
    BID_RETURN (res);
  }
} else if ((y.w[1] & MASK_INF) == MASK_INF) {
  // x is finite, so if y is positive infinity, then x is less, return 0
  //                 if y is negative infinity, then x is greater, return 1
  {
    res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}
  // if both numbers are zero, neither is greater => return NOTGREATERTHAN
if (x_is_zero && y_is_zero) {
  res = 1;
  BID_RETURN (res);
}
  // is x is zero, it is greater if Y is negative
else if (x_is_zero) {
  res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // is y is zero, X is greater if it is positive
else if (y_is_zero) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
  res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)
  // if exponents are the same, then we have a simple comparison 
  // of the significands
if (exp_y == exp_x) {
  res = (((sig_x.w[1] > sig_y.w[1])
	  || (sig_x.w[1] == sig_y.w[1]
	      && sig_x.w[0] >= sig_y.w[0])) ^ ((x.w[1] & MASK_SIGN) !=
					       MASK_SIGN));
  BID_RETURN (res);
}
  // if both components are either bigger or smaller, 
  // it is clear what needs to be done
if ((sig_x.w[1] > sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] > sig_y.w[0]))
    && exp_x >= exp_y) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
if ((sig_x.w[1] < sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] < sig_y.w[0]))
    && exp_x <= exp_y) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}

diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
if (diff > 0) {	// to simplify the loop below,

  // if exp_x is 33 greater than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }	// difference cannot be greater than 10^33

  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);


    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_y.w[1]
	&& (sig_n_prime256.w[0] == sig_y.w[0])) {
      res = 1;
      BID_RETURN (res);
    }	// if equal, return 0
    {
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN));
      BID_RETURN (res);
    }
  }
  //else { //128 by 64 bit multiply -> 192 bits
  __mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_x);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
      && (sig_n_prime192.w[0] == sig_y.w[0])) {
    res = 1;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res = (((sig_n_prime192.w[2] > 0)
	    || (sig_n_prime192.w[1] > sig_y.w[1])
	    || (sig_n_prime192.w[1] == sig_y.w[1]
		&& sig_n_prime192.w[0] >
		sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN));
    BID_RETURN (res);
  }
}

diff = exp_y - exp_x;

  // if exp_x is 33 less than exp_y, no need for compensation
if (diff > 33) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
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
    res = 1;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res =
      ((sig_n_prime256.w[3] != 0 || sig_n_prime256.w[2] != 0
	|| (sig_n_prime256.w[1] > sig_x.w[1]
	    || (sig_n_prime256.w[1] == sig_x.w[1]
		&& sig_n_prime256.w[0] >
		sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}
  //else { //128 by 64 bit multiply -> 192 bits
  // adjust the y significand upwards
__mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_y);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
    && (sig_n_prime192.w[0] == sig_x.w[0])) {
  res = 1;
  BID_RETURN (res);
}	// if equal, return 0
{
  res = (sig_n_prime192.w[2] != 0
	 || (sig_n_prime192.w[1] > sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] >
		 sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int,
					  bid128_signaling_less_unordered,
					  x, y)

     int res;
     int exp_x, exp_y;
     int diff;
     UINT128 sig_x, sig_y;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
*pfpsf |= INVALID_EXCEPTION;
{
  res = 1;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal.
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 0;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  // if x==neg_inf, { res = (y == neg_inf)?1:0; BID_RETURN (res) }
  if ((x.w[1] & MASK_SIGN) == MASK_SIGN)
    // x is -inf, so it is less than y unless y is -inf
  {
    res = (((y.w[1] & MASK_INF) != MASK_INF)
	   || (y.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  } else
    // x is pos_inf, no way for it to be less than y
  {
    res = 0;
    BID_RETURN (res);
  }
} else if ((y.w[1] & MASK_INF) == MASK_INF) {
  // x is finite, so if y is positive infinity, then x is less, return 0
  //                 if y is negative infinity, then x is greater, return 1
  {
    res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}
  // if both numbers are zero, neither is greater => return NOTGREATERTHAN
if (x_is_zero && y_is_zero) {
  res = 0;
  BID_RETURN (res);
}
  // is x is zero, it is greater if Y is negative
else if (x_is_zero) {
  res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // is y is zero, X is greater if it is positive
else if (y_is_zero) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
  res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)
  // if exponents are the same, then we have a simple comparison 
  // of the significands
if (exp_y == exp_x) {
  res = (((sig_x.w[1] > sig_y.w[1])
	  || (sig_x.w[1] == sig_y.w[1]
	      && sig_x.w[0] >= sig_y.w[0])) ^ ((x.w[1] & MASK_SIGN) !=
					       MASK_SIGN));
  BID_RETURN (res);
}
  // if both components are either bigger or smaller, 
  // it is clear what needs to be done
if ((sig_x.w[1] > sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] > sig_y.w[0]))
    && exp_x >= exp_y) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
if ((sig_x.w[1] < sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] < sig_y.w[0]))
    && exp_x <= exp_y) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}

diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
if (diff > 0) {	// to simplify the loop below,

  // if exp_x is 33 greater than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }	// difference cannot be greater than 10^33

  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);


    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_y.w[1]
	&& (sig_n_prime256.w[0] == sig_y.w[0])) {
      res = 0;
      BID_RETURN (res);
    }	// if equal, return 0
    {
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN));
      BID_RETURN (res);
    }
  }
  //else { //128 by 64 bit multiply -> 192 bits
  __mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_x);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
      && (sig_n_prime192.w[0] == sig_y.w[0])) {
    res = 0;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res = (((sig_n_prime192.w[2] > 0)
	    || (sig_n_prime192.w[1] > sig_y.w[1])
	    || (sig_n_prime192.w[1] == sig_y.w[1]
		&& sig_n_prime192.w[0] >
		sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN));
    BID_RETURN (res);
  }
}

diff = exp_y - exp_x;

  // if exp_x is 33 less than exp_y, no need for compensation
if (diff > 33) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
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
    res = 0;
    BID_RETURN (res);
  }	// if equal, return 1
  {
    res =
      ((sig_n_prime256.w[3] != 0 || sig_n_prime256.w[2] != 0
	|| (sig_n_prime256.w[1] > sig_x.w[1]
	    || (sig_n_prime256.w[1] == sig_x.w[1]
		&& sig_n_prime256.w[0] >
		sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}
  //else { //128 by 64 bit multiply -> 192 bits
  // adjust the y significand upwards
__mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_y);

  // if postitive, return whichever significand is larger (converse if negative)
if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
    && (sig_n_prime192.w[0] == sig_x.w[0])) {
  res = 0;
  BID_RETURN (res);
}	// if equal, return 0
{
  res = (sig_n_prime192.w[2] != 0
	 || (sig_n_prime192.w[1] > sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] >
		 sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int,
					  bid128_signaling_not_greater,
					  x, y)

     int res;
     int exp_x, exp_y;
     int diff;
     UINT128 sig_x, sig_y;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered, 
  // rather than equal : return 0
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
*pfpsf |= INVALID_EXCEPTION;
{
  res = 1;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal (not Greater).
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 1;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  // if x is neg infinity, there is no way it is greater than y, return 1
  if (((x.w[1] & MASK_SIGN) == MASK_SIGN)) {
    res = 1;
    BID_RETURN (res);
  }
  // x is pos infinity, it is greater, unless y is positive infinity => return y!=pos_infinity
  else {
    res = (((y.w[1] & MASK_INF) == MASK_INF)
	   && ((y.w[1] & MASK_SIGN) != MASK_SIGN));
    BID_RETURN (res);
  }
} else if ((y.w[1] & MASK_INF) == MASK_INF) {
  // x is finite, so if y is positive infinity, then x is less, return 0
  //                 if y is negative infinity, then x is greater, return 1
  {
    res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}
  // if both numbers are zero, neither is greater => return NOTGREATERTHAN
if (x_is_zero && y_is_zero) {
  res = 1;
  BID_RETURN (res);
}
  // is x is zero, it is greater if Y is negative
else if (x_is_zero) {
  res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // is y is zero, X is greater if it is positive
else if (y_is_zero) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
  res = ((y.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)
  // if exponents are the same, then we have a simple comparison 
  // of the significands
if (exp_y == exp_x) {
  res = (((sig_x.w[1] > sig_y.w[1])
	  || (sig_x.w[1] == sig_y.w[1]
	      && sig_x.w[0] >= sig_y.w[0])) ^ ((x.w[1] & MASK_SIGN) !=
					       MASK_SIGN));
  BID_RETURN (res);
}
  // if both components are either bigger or smaller, 
  // it is clear what needs to be done
if ((sig_x.w[1] > sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] > sig_y.w[0]))
    && exp_x >= exp_y) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
if ((sig_x.w[1] < sig_y.w[1]
     || (sig_x.w[1] == sig_y.w[1] && sig_x.w[0] < sig_y.w[0]))
    && exp_x <= exp_y) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}

diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
if (diff > 0) {	// to simplify the loop below,

  // if exp_x is 33 greater than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }	// difference cannot be greater than 10^33

  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);


    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_y.w[1]
	&& (sig_n_prime256.w[0] == sig_y.w[0])) {
      res = 1;
      BID_RETURN (res);
    }	// if equal, return 0
    {
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN));
      BID_RETURN (res);
    }
  }
  //else { //128 by 64 bit multiply -> 192 bits
  __mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_x);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
      && (sig_n_prime192.w[0] == sig_y.w[0])) {
    res = 1;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res = (((sig_n_prime192.w[2] > 0)
	    || (sig_n_prime192.w[1] > sig_y.w[1])
	    || (sig_n_prime192.w[1] == sig_y.w[1]
		&& sig_n_prime192.w[0] >
		sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) != MASK_SIGN));
    BID_RETURN (res);
  }
}

diff = exp_y - exp_x;

  // if exp_x is 33 less than exp_y, no need for compensation
if (diff > 33) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
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
    res = 1;
    BID_RETURN (res);
  }	// if equal, return 0
  {
    res =
      ((sig_n_prime256.w[3] != 0 || sig_n_prime256.w[2] != 0
	|| (sig_n_prime256.w[1] > sig_x.w[1]
	    || (sig_n_prime256.w[1] == sig_x.w[1]
		&& sig_n_prime256.w[0] >
		sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}
  //else { //128 by 64 bit multiply -> 192 bits
  // adjust the y significand upwards
__mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_y);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
    && (sig_n_prime192.w[0] == sig_x.w[0])) {
  res = 1;
  BID_RETURN (res);
}	// if equal, return 0
{
  res = (sig_n_prime192.w[2] != 0
	 || (sig_n_prime192.w[1] > sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] >
		 sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
}

BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE (int,
					  bid128_signaling_not_less, x,
					  y)

     int res;
     int exp_x, exp_y;
     int diff;
     UINT128 sig_x, sig_y;
     UINT192 sig_n_prime192;
     UINT256 sig_n_prime256;
     char x_is_zero = 0, y_is_zero = 0, non_canon_x, non_canon_y;

  // NaN (CASE1)
  // if either number is NAN, the comparison is unordered, 
  // rather than equal : return 1
if (((x.w[1] & MASK_NAN) == MASK_NAN)
    || ((y.w[1] & MASK_NAN) == MASK_NAN)) {
*pfpsf |= INVALID_EXCEPTION;
{
  res = 1;
  BID_RETURN (res);
}
}
  // SIMPLE (CASE2)
  // if all the bits are the same, these numbers are equal (not Greater).
if (x.w[0] == y.w[0] && x.w[1] == y.w[1]) {
  res = 1;
  BID_RETURN (res);
}
  // INFINITY (CASE3)
if ((x.w[1] & MASK_INF) == MASK_INF) {
  // if x==neg_inf, { res = (y == neg_inf)?1:0; BID_RETURN (res) }
  if ((x.w[1] & MASK_SIGN) == MASK_SIGN)
    // x is -inf, so it is less than y unless y is -inf
  {
    res = (((y.w[1] & MASK_INF) == MASK_INF)
	   && (y.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  } else
    // x is pos_inf, no way for it to be less than y
  {
    res = 1;
    BID_RETURN (res);
  }
} else if ((y.w[1] & MASK_INF) == MASK_INF) {
  // x is finite, so if y is positive infinity, then x is less, return 0
  //                 if y is negative infinity, then x is greater, return 1
  {
    res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
    BID_RETURN (res);
  }
}
  // CONVERT X
sig_x.w[1] = x.w[1] & 0x0001ffffffffffffull;
sig_x.w[0] = x.w[0];
exp_x = (x.w[1] >> 49) & 0x000000000003fffull;

  // CHECK IF X IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_x.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_x.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_x.w[0] > 0x378d8e63ffffffffull))
    || ((x.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_x = 1;
else
  non_canon_x = 0;

  // CONVERT Y
exp_y = (y.w[1] >> 49) & 0x0000000000003fffull;
sig_y.w[1] = y.w[1] & 0x0001ffffffffffffull;
sig_y.w[0] = y.w[0];

  // CHECK IF Y IS CANONICAL
  // 9999999999999999999999999999999999(decimal) = 
  //   1ed09_bead87c0_378d8e63_ffffffff(hexadecimal)
  // [0, 10^34) is the 754r supported canonical range.  
  //     If the value exceeds that, it is interpreted as 0.
if ((sig_y.w[1] > 0x0001ed09bead87c0ull)
    || ((sig_y.w[1] == 0x0001ed09bead87c0ull)
	&& (sig_y.w[0] > 0x378d8e63ffffffffull))
    || ((y.w[1] & 0x6000000000000000ull) == 0x6000000000000000ull))
  non_canon_y = 1;
else
  non_canon_y = 0;

  // ZERO (CASE4)
  // some properties:
  //    (+ZERO == -ZERO) => therefore ignore the sign
  //    (ZERO x 10^A == ZERO x 10^B) for any valid A, B => therefore 
  //    ignore the exponent field
  //    (Any non-canonical # is considered 0)
if (non_canon_x || ((sig_x.w[1] == 0) && (sig_x.w[0] == 0))) {
  x_is_zero = 1;
}
if (non_canon_y || ((sig_y.w[1] == 0) && (sig_y.w[0] == 0))) {
  y_is_zero = 1;
}
  // if both numbers are zero, neither is greater => return NOTGREATERTHAN
if (x_is_zero && y_is_zero) {
  res = 1;
  BID_RETURN (res);
}
  // is x is zero, it is greater if Y is negative
else if (x_is_zero) {
  res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // is y is zero, X is greater if it is positive
else if (y_is_zero) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
  // OPPOSITE SIGN (CASE5)
  // now, if the sign bits differ, x is greater if y is negative
if (((x.w[1] ^ y.w[1]) & MASK_SIGN) == MASK_SIGN) {
  res = ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
  // REDUNDANT REPRESENTATIONS (CASE6)

  // if exponents are the same, then we have a simple comparison 
  // of the significands
if (exp_y == exp_x) {
  res = (((sig_x.w[1] > sig_y.w[1])
	  || (sig_x.w[1] == sig_y.w[1]
	      && sig_x.w[0] >= sig_y.w[0])) ^ ((x.w[1] & MASK_SIGN) ==
					       MASK_SIGN));
  BID_RETURN (res);
}
  // if both components are either bigger or smaller, 
  // it is clear what needs to be done
if (sig_x.w[1] >= sig_y.w[1] && sig_x.w[0] >= sig_y.w[0]
    && exp_x > exp_y) {
  res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
  BID_RETURN (res);
}
if (sig_x.w[1] <= sig_y.w[1] && sig_x.w[0] <= sig_y.w[0]
    && exp_x < exp_y) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}

diff = exp_x - exp_y;

  // if |exp_x - exp_y| < 33, it comes down to the compensated significand
if (diff > 0) {	// to simplify the loop below,

  // if exp_x is 33 greater than exp_y, no need for compensation
  if (diff > 33) {
    res = ((x.w[1] & MASK_SIGN) != MASK_SIGN);
    BID_RETURN (res);
  }	// difference cannot be greater than 10^33

  if (diff > 19) {	//128 by 128 bit multiply -> 256 bits
    __mul_128x128_to_256 (sig_n_prime256, sig_x, ten2k128[diff - 20]);


    // if postitive, return whichever significand is larger 
    // (converse if negative)
    if (sig_n_prime256.w[3] == 0 && (sig_n_prime256.w[2] == 0)
	&& sig_n_prime256.w[1] == sig_y.w[1]
	&& (sig_n_prime256.w[0] == sig_y.w[0])) {
      res = 1;
      BID_RETURN (res);
    }	// if equal, return 1
    {
      res = ((((sig_n_prime256.w[3] > 0) || sig_n_prime256.w[2] > 0)
	      || (sig_n_prime256.w[1] > sig_y.w[1])
	      || (sig_n_prime256.w[1] == sig_y.w[1]
		  && sig_n_prime256.w[0] >
		  sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN));
      BID_RETURN (res);
    }
  }
  //else { //128 by 64 bit multiply -> 192 bits
  __mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_x);

  // if postitive, return whichever significand is larger 
  // (converse if negative)
  if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_y.w[1]
      && (sig_n_prime192.w[0] == sig_y.w[0])) {
    res = 1;
    BID_RETURN (res);
  }	// if equal, return 1
  {
    res = (((sig_n_prime192.w[2] > 0)
	    || (sig_n_prime192.w[1] > sig_y.w[1])
	    || (sig_n_prime192.w[1] == sig_y.w[1]
		&& sig_n_prime192.w[0] >
		sig_y.w[0])) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}

diff = exp_y - exp_x;

  // if exp_x is 33 less than exp_y, no need for compensation
if (diff > 33) {
  res = ((x.w[1] & MASK_SIGN) == MASK_SIGN);
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
    res = 1;
    BID_RETURN (res);
  }	// if equal, return 1
  {
    res =
      ((sig_n_prime256.w[3] == 0 && sig_n_prime256.w[2] == 0
	&& (sig_n_prime256.w[1] < sig_x.w[1]
	    || (sig_n_prime256.w[1] == sig_x.w[1]
		&& sig_n_prime256.w[0] <
		sig_x.w[0]))) ^ ((x.w[1] & MASK_SIGN) == MASK_SIGN));
    BID_RETURN (res);
  }
}
  //else { //128 by 64 bit multiply -> 192 bits
  // adjust the y significand upwards
__mul_64x128_to192 (sig_n_prime192, ten2k64[diff], sig_y);

  // if postitive, return whichever significand is larger (converse if negative)
if ((sig_n_prime192.w[2] == 0) && sig_n_prime192.w[1] == sig_x.w[1]
    && (sig_n_prime192.w[0] == sig_x.w[0])) {
  res = 1;
  BID_RETURN (res);
}	// if equal, return 1
{
  res = (sig_n_prime192.w[2] == 0
	 && (sig_n_prime192.w[1] < sig_x.w[1]
	     || (sig_n_prime192.w[1] == sig_x.w[1]
		 && sig_n_prime192.w[0] <
		 sig_x.w[0]))) ^ ((y.w[1] & MASK_SIGN) == MASK_SIGN);
  BID_RETURN (res);
}
}
