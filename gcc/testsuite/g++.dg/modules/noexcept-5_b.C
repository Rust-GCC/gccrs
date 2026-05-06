// PR c++/124785
// { dg-do compile { target c++20 } }
// { dg-additional-options "-fmodules" }
// { dg-module-cmi M:B }

export module M:B;
#include "noexcept-5.h"
