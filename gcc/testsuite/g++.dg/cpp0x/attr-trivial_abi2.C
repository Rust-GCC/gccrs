// { dg-do compile { target c++11 } }
// Test error cases and restrictions for __attribute__((trivial_abi)) attribute

// Test: attribute rejected on non-class types
int __attribute__ ((trivial_abi))
x; // { dg-warning "'trivial_abi' attribute only applies to classes" }
typedef int __attribute__ ((trivial_abi))
int_t; // { dg-warning "'trivial_abi' attribute only applies to classes" }

// Test: attribute rejected on types with virtual functions
struct __attribute__ ((trivial_abi))
WithVirtual // { dg-warning "'trivial_abi' cannot be applied to 'WithVirtual'" }
{
  virtual void f () {}
};

// Test: attribute rejected on types with virtual bases
struct Base
{
};

struct __attribute__ ((trivial_abi))
WithVirtualBase // { dg-warning "'trivial_abi' cannot be applied" }
  : virtual Base
{
  int x;
};

// Test: attribute rejected when all copy/move constructors are deleted
struct __attribute__ ((trivial_abi))
AllDeleted // { dg-warning "'trivial_abi' cannot be applied to 'AllDeleted'" }
{
  AllDeleted (const AllDeleted &) = delete;
  AllDeleted (AllDeleted &&) = delete;
  int x;
};

// Test: attribute rejected on types with non-trivial base classes
struct NonTrivialBase
{
  NonTrivialBase (const NonTrivialBase &) {} // Non-trivial copy
};

struct __attribute__ ((trivial_abi))
WithNonTrivialBase // { dg-warning "'trivial_abi' cannot be applied" }
  : NonTrivialBase
{
  int x;
};

// Test: attribute rejected on types with non-trivial member types
struct NonTrivialMember
{
  NonTrivialMember (const NonTrivialMember &) {} // Non-trivial copy
};

struct __attribute__ ((trivial_abi))
WithNonTrivialMember // { dg-warning "'trivial_abi' cannot be applied" }
{
  NonTrivialMember member;
  int x;
};

// Test: attribute rejected on array members of non-trivial types
struct __attribute__ ((trivial_abi))
WithNonTrivialArray // { dg-warning "'trivial_abi' cannot be applied" }
{
  NonTrivialMember arr[5];
  int x;
};
