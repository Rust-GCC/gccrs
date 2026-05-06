// { dg-do compile { target c++26 } }

#include <functional>

using std::constant_wrapper;
using std::function_ref;

using std::is_nothrow_move_assignable_v;
using std::is_nothrow_copy_assignable_v;
using std::is_nothrow_assignable_v;
using std::is_assignable_v;
using std::is_nothrow_swappable_v;
using std::is_trivially_copyable_v;

static_assert( is_nothrow_move_assignable_v<function_ref<void()>> );
static_assert( is_nothrow_copy_assignable_v<function_ref<void()>> );
static_assert( is_nothrow_swappable_v<function_ref<void()>> );

static_assert( ! is_assignable_v<function_ref<void()>, std::nullptr_t> );

static_assert( is_nothrow_assignable_v<function_ref<void()>, void()> );
static_assert( is_nothrow_assignable_v<function_ref<void()>, void(&)()> );
static_assert( is_nothrow_assignable_v<function_ref<void()>, void(*)()> );
static_assert( is_nothrow_assignable_v<function_ref<void()>, int()> );
static_assert( is_nothrow_assignable_v<function_ref<void()>, int(&)()> );
static_assert( is_nothrow_assignable_v<function_ref<void()>, int(*)()> );
static_assert( ! is_nothrow_assignable_v<function_ref<void()>, void(int)> );
static_assert( is_nothrow_assignable_v<function_ref<void(int)>, void(int)> );

static_assert( is_nothrow_assignable_v<function_ref<void()>,
				       void() noexcept> );
static_assert( is_nothrow_assignable_v<function_ref<void() noexcept>,
					void() noexcept> );
static_assert( ! is_assignable_v<function_ref<void() noexcept>, void() > );

struct S
{
  int x;
  int f();
};
int funS(S);

static_assert( is_nothrow_assignable_v<function_ref<int(S)>,
				       decltype(funS)> );
static_assert( is_nothrow_assignable_v<function_ref<int(S)>,
				       decltype(&funS)> );
static_assert( ! is_assignable_v<function_ref<int(S)>, decltype(&S::x)> );
static_assert( ! is_assignable_v<function_ref<int(S)>, decltype(&S::f)> );

static_assert( is_nothrow_assignable_v<function_ref<int(S)>,
				       constant_wrapper<funS>> );
static_assert( is_nothrow_assignable_v<function_ref<int(S)>,
				       constant_wrapper<&funS>> );
static_assert( is_nothrow_assignable_v<function_ref<int(S)>,
				       constant_wrapper<&S::x>> );
static_assert( is_nothrow_assignable_v<function_ref<int(S)>,
				       constant_wrapper<&S::f>> );
struct Q
{
  void operator()() const;
};

static_assert( ! is_assignable_v<function_ref<void()>, Q> );
static_assert( ! is_assignable_v<function_ref<void()>, Q&> );
static_assert( ! is_assignable_v<function_ref<void()>, const Q&> );
static_assert( ! is_assignable_v<function_ref<void() const>, Q> );
static_assert( ! is_assignable_v<function_ref<void() const>, Q&> );
static_assert( ! is_assignable_v<function_ref<void() const>, const Q&> );

static_assert( is_nothrow_assignable_v<function_ref<void()>,
				       constant_wrapper<Q{}>> );
static_assert( is_nothrow_assignable_v<function_ref<void() const>,
				       constant_wrapper<Q{}>> );

constexpr bool
test_constexpr()
{
  function_ref<void(S)> fp(std::cw<funS>);
  fp = std::cw<funS>;
  fp = std::cw<&funS>;
  fp = std::cw<&S::x>;
  fp = std::cw<&S::f>;

  constexpr Q cq;
  function_ref<void() const> fq(cq);
  fq = std::cw<cq>;
  return true;
}
static_assert( test_constexpr() );

void func();

void
test_instantiation()
{
  function_ref<void(S)> fp(funS);
  fp = funS;
  fp = &funS;

  test_constexpr();
}
