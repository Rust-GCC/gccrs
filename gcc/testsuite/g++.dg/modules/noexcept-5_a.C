// PR c++/124785
// { dg-do compile { target c++20 } }
// { dg-additional-options "-fmodules" }
// { dg-module-cmi M:A }

export module M:A;
#include "noexcept-5.h"
