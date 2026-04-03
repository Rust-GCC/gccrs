// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }
// Test reflection type traits [meta.reflection.traits], type properties.

#include <meta>
using namespace std::meta;

struct A { constexpr A () : a (42) {} mutable int a; };
struct B { constexpr B () {} A b; };
struct C : public A { constexpr C () {} };
struct D {};
struct E : protected D { constexpr E () {} };
struct F { constexpr F () : f (42) {} int f; };
struct G { F a; int b[2]; constexpr G () : b{0,1} {} };
enum EnumType { e0 };
class ClassType { };
struct NType { int i; int j; virtual ~NType (); };
struct TType { int i; private: int j; };
struct SLType { int i; int j; ~SLType (); };
struct PODType { int i; int j; };
struct LType { int _M_i; constexpr LType (int __i) : _M_i(__i) { } };
struct LTypeDerived : public LType { constexpr LTypeDerived (int __i) : LType(__i) { } };
struct NLType {
  int _M_i;
  NLType () : _M_i(0) { }
  constexpr NLType (int __i) : _M_i(__i) { }
  NLType (const NLType &__other) : _M_i(__other._M_i) { }
  ~NLType () { _M_i = 0; }
};

static_assert (is_structural_type (^^int));
static_assert (is_structural_type (^^float));
static_assert (is_structural_type (^^const float));
static_assert (is_structural_type (^^EnumType));
static_assert (is_structural_type (^^int*));
static_assert (is_structural_type (^^int(*)(int)));
static_assert (is_structural_type (^^int (ClassType::*)));
static_assert (is_structural_type (^^int (ClassType::*) (int)));
static_assert (is_structural_type (^^std::nullptr_t));
static_assert (is_structural_type (^^info));
static_assert (is_structural_type (^^int&));
static_assert (is_structural_type (^^NLType&));
static_assert (is_structural_type (^^PODType));
static_assert (is_structural_type (^^const LType));
static_assert (is_structural_type (^^LTypeDerived));
static_assert (is_structural_type (^^D));
static_assert (is_structural_type (^^F));
static_assert (is_structural_type (^^G));

static_assert (!is_structural_type (^^int&&));
static_assert (!is_structural_type (^^int[2]));
static_assert (!is_structural_type (^^TType));
static_assert (!is_structural_type (^^NType));
static_assert (!is_structural_type (^^SLType));
static_assert (!is_structural_type (^^LType[5]));
static_assert (!is_structural_type (^^NLType));
static_assert (!is_structural_type (^^NLType[5]));
static_assert (!is_structural_type (^^A));
static_assert (!is_structural_type (^^const B));
static_assert (!is_structural_type (^^C));
static_assert (!is_structural_type (^^E));
