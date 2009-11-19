// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

package math


/*
 *	sqrt returns the square root of its floating
 *	point argument. Newton's method.
 *
 *	calls frexp
 */

// Sqrt returns the square root of x.
//
// Special cases are:
//	Sqrt(+Inf) = +Inf
//	Sqrt(0) = 0
//	Sqrt(x < 0) = NaN
func Sqrt(x float64) float64 {
	if IsInf(x, 1) {
		return x
	}

	if x <= 0 {
		if x < 0 {
			return NaN()
		}
		return 0;
	}

	return __builtin_sqrt(x);
}
