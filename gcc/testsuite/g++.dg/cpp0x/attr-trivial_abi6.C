// { dg-do compile { target c++11 } }
// Test that [[gnu::trivial_abi]] is rejected

struct [[gnu::trivial_abi]] S0 // { dg-warning "'..gnu::trivial_abi..' is not supported" }
{
  int a;
  ~S0 () {}
};

// __attribute__((trivial_abi)) should still work
struct __attribute__ ((trivial_abi)) S1
{
  int a;
  ~S1 () {}
};

// [[clang::trivial_abi]] should still work
struct [[clang::trivial_abi]] S2
{
  int a;
  ~S2 () {}
};
