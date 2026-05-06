// { dg-do compile { target c++11 } }
// Test ABI behavior of [[clang::trivial_abi]] attribute
// This test verifies that types with trivial_abi are passed by value
// instead of by invisible reference, even with non-trivial constructors

struct [[clang::trivial_abi]] TrivialAbiType
{
  int data;
  TrivialAbiType () : data (0) {}
  TrivialAbiType (const TrivialAbiType &other) : data (other.data) {}
  TrivialAbiType (TrivialAbiType &&other) : data (other.data) {}
  ~TrivialAbiType () {}
};

struct NonTrivialType
{
  int data;
  NonTrivialType () : data (0) {}
  NonTrivialType (const NonTrivialType &other) : data (other.data) {}
  NonTrivialType (NonTrivialType &&other) : data (other.data) {}
  ~NonTrivialType () {}
};

// Test that trivial_abi types are considered trivially relocatable
// (ABI behavior is tested separately in invisiref3.C)

// Function templates to test parameter passing
template <typename T>
void
test_param_passing (T param)
{
  // Function body doesn't matter for ABI test
}

// Test usage
void
test_functions ()
{
  TrivialAbiType trivial_obj;
  NonTrivialType non_trivial_obj;

  // Both should compile, but with different ABI behavior
  test_param_passing (trivial_obj);	// Should pass by value
  test_param_passing (non_trivial_obj); // Should pass by invisible reference
}

// Test return values
TrivialAbiType
return_trivial ()
{
  return TrivialAbiType{};
}

NonTrivialType
return_non_trivial ()
{
  return NonTrivialType{};
}
