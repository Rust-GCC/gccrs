/* Implementation of the NXConstantString class for Objective-C.
   Copyright (C) 1995-2023 Free Software Foundation, Inc.
   Contributed by Pieter J. Schoenmakers <tiggr@es.ele.tue.nl>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */ 

#include "objc-private/common.h"
#include "objc/NXConstStr.h"

@implementation NXConstantString

-(const char *) cString
{
  return (c_string);
} /* -cString */

-(unsigned int) length
{
  return (len);
} /* -length */

@end
