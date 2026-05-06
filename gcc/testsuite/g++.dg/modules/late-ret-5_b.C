// PR c++/118630
// { dg-do compile { target c++20 } }
// { dg-additional-options "-fmodules -fno-module-lazy" }

#include "late-ret-5.h"
import "late-ret-5_a.H";
