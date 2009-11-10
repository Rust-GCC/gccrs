/* go-panic.h -- declare the go panic statement.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

extern void __go_panic(const char* format, ...)
  __attribute__ ((format (printf, 1, 2)))
  __attribute__ ((nonnull (1)))
  __attribute__ ((noreturn));
