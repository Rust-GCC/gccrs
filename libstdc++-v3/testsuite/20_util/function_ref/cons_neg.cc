// { dg-do compile { target c++26 } }

#include <functional>

using std::function_ref;

struct S
{
  int x;
  void foo();
};
S s;

constexpr int(*fp)(S) = nullptr;
constexpr int S::*mdp = nullptr;
constexpr int (S::*mfp)() = nullptr;

function_ref<int(S)> fd1(std::cw<fp>);  // { dg-error "from here" }
function_ref<int(S)> fd2(std::cw<mdp>); // { dg-error "from here" }
function_ref<int(S)> fd3(std::cw<mfp>); // { dg-error "from here" }

function_ref<int()> br4(std::cw<fp>, s);  // { dg-error "from here" }
function_ref<int()> br5(std::cw<mdp>, s); // { dg-error "from here" }
function_ref<int()> br6(std::cw<mfp>, s); // { dg-error "from here" }

function_ref<int()> bp7(std::cw<mdp>, &s); // { dg-error "from here" }
function_ref<int()> bp8(std::cw<mfp>, &s); // { dg-error "from here" }

// { dg-prune-output "static assertion failed" }
