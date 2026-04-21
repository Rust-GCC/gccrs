// { dg-do compile { target c++11 } }
// Test edge cases and inheritance with [[clang::trivial_abi]] attribute

// Test: Derived classes with trivial_abi base classes
struct [[clang::trivial_abi]] TrivialBase
{
  int x;
  ~TrivialBase () {}
};

// This should be allowed - base class has trivial_abi
struct [[clang::trivial_abi]] DerivedFromTrivial : TrivialBase
{
  int y;
  ~DerivedFromTrivial () {}
};

// Test: Multiple inheritance with all trivial bases
struct [[clang::trivial_abi]] TrivialBase2
{
  int z;
  ~TrivialBase2 () {}
};

struct [[clang::trivial_abi]] MultipleInheritance : TrivialBase, TrivialBase2
{
  int w;
  ~MultipleInheritance () {}
};

// Test: Empty class with trivial_abi
struct [[clang::trivial_abi]] EmptyTrivialAbi
{
  ~EmptyTrivialAbi () {}
};

// Test: Class with only trivial members
struct [[clang::trivial_abi]] OnlyTrivialMembers
{
  int a;
  float b;
  char c[10];
  ~OnlyTrivialMembers () {}
};

// Test: Template class with trivial_abi
template <typename T> struct [[clang::trivial_abi]] TemplateTrivialAbi
{
  T data;
  ~TemplateTrivialAbi () {}
};

// This should work for scalar types
using IntTrivialAbi = TemplateTrivialAbi<int>;

// Test with another trivial_abi type as template parameter
using NestedTrivialAbi = TemplateTrivialAbi<TrivialBase>;
