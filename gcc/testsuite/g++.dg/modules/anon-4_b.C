// PR c++/124582
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fmodules -fno-module-lazy -freflection" }

#include "anon-4.h"
import "anon-4_a.H";
