// { dg-do compile { target c++11 } }
// Test basic functionality of [[clang::trivial_abi]] attribute

struct [[clang::trivial_abi]] TrivialAbi1
{
  int x;
  ~TrivialAbi1 () {
  } // Non-trivial destructor, but should be allowed with trivial_abi
};

class [[clang::trivial_abi]] TrivialAbi2
{
  int y;

public:
  TrivialAbi2 (const TrivialAbi2 &) {
  } // Non-trivial copy constructor, but should be allowed
  ~TrivialAbi2 () {}
};

// Test that the attribute is recognized and the types compile successfully

// Test basic usage in function parameters and return values
TrivialAbi1
func1 (TrivialAbi1 param)
{
  return param;
}

TrivialAbi2
func2 (TrivialAbi2 param)
{
  return param;
}

union [[clang::trivial_abi]] TrivialUnion
{
  int a;
  float b;
  ~TrivialUnion () {}
};
