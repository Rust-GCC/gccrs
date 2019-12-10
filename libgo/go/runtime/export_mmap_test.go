// Copyright 2016 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// +build aix darwin dragonfly freebsd hurd linux nacl netbsd openbsd solaris

// Export guts for testing.

package runtime

var Mmap = mmap
var Munmap = munmap
var Errno = errno

const ENOMEM = _ENOMEM
const MAP_ANON = _MAP_ANON
const MAP_PRIVATE = _MAP_PRIVATE
const MAP_FIXED = _MAP_FIXED

func GetPhysPageSize() uintptr {
	return physPageSize
}
