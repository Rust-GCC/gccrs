// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

package math

func libc_sqrt(float64) float64 __asm__("sqrt")

// Sqrt returns the square root of x.
//
// Special cases are:
//	Sqrt(+Inf) = +Inf
//	Sqrt(x < 0) = NaN
//	Sqrt(NaN) = NaN
func Sqrt(x float64) float64 {
	if IsInf(x, 1) {
		return x
	}

	if x <= 0 {
		if x < 0 {
			return NaN()
		}
		return 0
	}

	if IsNaN(x) {
		return NaN()
	}

	return libc_sqrt(x)
}
