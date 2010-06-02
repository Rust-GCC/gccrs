/* go-panic.h -- declare the go panic statement.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

extern void __go_panic (void)
  __attribute__ ((noreturn));

extern void __go_panic_msg (const char* msg)
  __attribute__ ((noreturn));

extern void __go_print_msg (_Bool is_panic, const char* msg);

struct __go_string;
extern void __go_print_string (_Bool is_panic, struct __go_string);
