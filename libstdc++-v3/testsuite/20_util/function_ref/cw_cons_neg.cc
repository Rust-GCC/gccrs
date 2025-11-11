// { dg-do compile { target c++26 } }

#include <functional>

using std::function_ref;
using std::constant_wrapper;

struct S
{
  int operator()() const { return 1; }

  // Non-constant, so cw<S{}>(cw<1>) call never unwrapps
  int operator()(int) const { return 1; }
  template<auto __cw>
    int operator()(constant_wrapper<__cw, int>) const { return 1; }

  // Constant, cw<S{}>(cw<1>, cw<2>) calls int overload
  // while S{}(cw<1>, cw<1>) calls constant_wrapper overload
  constexpr int operator()(int, int) const { return 1; }
  template<auto __cw1, auto __cw2>
    constexpr int operator()(constant_wrapper<__cw1, int>,
			     constant_wrapper<__cw2, int>)
    { return 1; }
};

function_ref<int()> f0a(S{});

function_ref<int(int)> f1a(S{});
function_ref<int(constant_wrapper<2>)> f1b(std::cw<S{}>);

function_ref<int(int, int)> f2a(std::cw<S{}>); // OK, runtime
function_ref<int(constant_wrapper<1>, int)> f2b(std::cw<S{}>); // OK, still runtime
function_ref<int(constant_wrapper<2>, constant_wrapper<3>)> f2c(std::cw<S{}>); // { dg-error "from here" }

// { dg-prune-output "static assertion failed" }
