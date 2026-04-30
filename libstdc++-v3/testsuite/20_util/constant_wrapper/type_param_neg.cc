// { dg-do compile { target c++26 } }
#include <utility>

std::constant_wrapper<1, float> c1;      // { dg-error "from here" }
std::constant_wrapper<1.0, int> c2;      // { dg-error "from here" }
std::constant_wrapper<1, int const> c3;  // { dg-error "from here" }
std::constant_wrapper<1, int const&> c4; // { dg-error "from here" }

// { dg-prune-output "static assertion failed" }
