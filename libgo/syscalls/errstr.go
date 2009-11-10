// errstr.go -- Error strings.

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

package syscall

const ENONE = 0;

// FIXME: The name is only right for glibc.
func libc_strerror_r(int, *byte, Size_t) int __asm__ ("__xpg_strerror_r")
func errno_location() *int __asm__ ("__errno_location");

func Errstr(errno int) string {
	for len := Size_t(128); ; len *= 2 {
		b := make([]byte, len);
		r := libc_strerror_r(errno, &b[0], len);
		if r >= 0 {
			i := 0;
			for b[i] != 0 {
				i++;
			}
			return string(b[0:i]);
		}
		if *errno_location() != ERANGE {
			return "Errstr failure";
		}
	}
}
