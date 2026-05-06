// { dg-do compile { target c++11 } }
// Test compatibility of [[clang::trivial_abi]] with other GCC features

// Test: Compatibility with other attributes
struct [[clang::trivial_abi]] [[gnu::packed]] TrivialAbiPacked
{
  char a;
  int b;
  ~TrivialAbiPacked () {}
};

// Test: Compatibility with alignas
struct [[clang::trivial_abi]] alignas (16) TrivialAbiAligned
{
  int x;
  ~TrivialAbiAligned () {}
};

// Test: Forward declaration and later definition
struct TrivialForward;
struct [[clang::trivial_abi]] TrivialForward
{
  int x;
  ~TrivialForward () {}
};

// Test: typedef and using declarations
using TrivialAlias = TrivialAbiPacked;
typedef TrivialAbiAligned TrivialTypedef;

// Test: Local classes
void
test_local_class ()
{
  struct [[clang::trivial_abi]] LocalTrivial
  {
    int x;
    ~LocalTrivial () {}
  };
}

// Test: Named nested structs with trivial_abi
struct ContainerClass
{
  struct [[clang::trivial_abi]] NestedStruct
  {
    int nested_member;
    ~NestedStruct () {}
  };
};

// Test: Nested classes
struct Outer
{
  struct [[clang::trivial_abi]] Nested
  {
    int value;
    ~Nested () {}
  };
};
