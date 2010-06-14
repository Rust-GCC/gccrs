/* go-defer-stack.c -- The defer stack.

   Copyright 2010 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include "go-defer.h"

__thread struct __defer_stack *__go_defer_stack;
