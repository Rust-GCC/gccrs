// { dg-do compile { target c++26 } }
// { dg-add-options no_pch }

#include <functional>

#ifndef __cpp_lib_function_ref
# error "Feature-test macro for function_ref missing in <functional>"
#elif __cpp_lib_function_ref != 202306L
# error "Feature-test macro for function_ref has wrong value in <functional>"
#endif

using std::constant_wrapper;
using std::function_ref;

using std::is_default_constructible_v;
using std::is_nothrow_copy_constructible_v;
using std::is_nothrow_move_constructible_v;
using std::is_nothrow_constructible_v;
using std::is_constructible_v;
using std::is_trivially_copyable_v;

static_assert( ! is_default_constructible_v<function_ref<void()>> );
static_assert( is_nothrow_move_constructible_v<function_ref<void()>> );
static_assert( is_nothrow_copy_constructible_v<function_ref<void()>> );
static_assert( is_trivially_copyable_v<function_ref<void()>> );

static_assert( ! is_constructible_v<function_ref<void()>, std::nullptr_t> );

static_assert( is_nothrow_constructible_v<function_ref<void()>, void()> );
static_assert( is_nothrow_constructible_v<function_ref<void()>, void(&)()> );
static_assert( is_nothrow_constructible_v<function_ref<void()>, void(*)()> );
static_assert( is_nothrow_constructible_v<function_ref<void()>, int()> );
static_assert( is_nothrow_constructible_v<function_ref<void()>, int(&)()> );
static_assert( is_nothrow_constructible_v<function_ref<void()>, int(*)()> );
static_assert( ! is_constructible_v<function_ref<void()>, void(int)> );
static_assert( is_nothrow_constructible_v<function_ref<void(int)>, void(int)> );

static_assert( is_nothrow_constructible_v<function_ref<void()>,
					  void() noexcept> );
static_assert( is_nothrow_constructible_v<function_ref<void() noexcept>,
					  void() noexcept> );
static_assert( ! is_constructible_v<function_ref<void() noexcept>,
				    void() > );

struct S
{
  int x;
  int f();
};
int funS(S);

static_assert( is_nothrow_constructible_v<function_ref<int(S)>,
					  decltype(funS)> );
static_assert( is_nothrow_constructible_v<function_ref<int(S)>,
					  decltype(&funS)> );
static_assert( ! is_constructible_v<function_ref<int(S)>,
				    decltype(&S::x)> );
static_assert( ! is_constructible_v<function_ref<int(S)>,
				    decltype(&S::f)> );

static_assert( is_nothrow_constructible_v<function_ref<int(S)>,
					  constant_wrapper<funS>> );
static_assert( is_nothrow_constructible_v<function_ref<int(S)>,
					  constant_wrapper<&funS>> );
static_assert( is_nothrow_constructible_v<function_ref<int(S)>,
					  constant_wrapper<&S::x>> );
static_assert( is_nothrow_constructible_v<function_ref<int(S)>,
					  constant_wrapper<&S::f>> );

static_assert( is_nothrow_constructible_v<function_ref<int()>,
					  constant_wrapper<funS>, S&> );
static_assert( is_nothrow_constructible_v<function_ref<int()>,
					  constant_wrapper<&funS>, S&> );
static_assert( is_nothrow_constructible_v<function_ref<int()>,
					  constant_wrapper<&S::x>, S&> );
static_assert( is_nothrow_constructible_v<function_ref<int()>,
					  constant_wrapper<&S::f>, S&> );

static_assert( ! is_constructible_v<function_ref<int()>,
				    constant_wrapper<funS>, S*> );
static_assert( ! is_constructible_v<function_ref<int()>,
				    constant_wrapper<&funS>, S*> );
static_assert( is_nothrow_constructible_v<function_ref<int()>,
					  constant_wrapper<&S::x>, S*> );
static_assert( is_nothrow_constructible_v<function_ref<int()>,
					  constant_wrapper<&S::f>, S*> );

struct M
{
  void operator()();
};


static_assert( is_nothrow_constructible_v<function_ref<void()>, M> );
static_assert( is_nothrow_constructible_v<function_ref<void()>, M&> );
static_assert( ! is_constructible_v<function_ref<void()>, const M&> );
static_assert( ! is_constructible_v<function_ref<void() const>, M> );
static_assert( ! is_constructible_v<function_ref<void() const>, const M&> );
static_assert( ! is_constructible_v<function_ref<void()>,
				    constant_wrapper<M{}>> );
static_assert( ! is_constructible_v<function_ref<void() const>,
				    constant_wrapper<M{}>> );
struct Q
{
  void operator()(int) const;
  void operator()(int*) const;
};

static_assert( is_nothrow_constructible_v<function_ref<void(int)>, Q> );
static_assert( is_nothrow_constructible_v<function_ref<void(int)>, Q&> );
static_assert( is_nothrow_constructible_v<function_ref<void(int)>, const Q&> );
static_assert( is_nothrow_constructible_v<function_ref<void(int) const>, Q> );
static_assert( is_nothrow_constructible_v<function_ref<void(int) const>, Q&> );
static_assert( is_nothrow_constructible_v<function_ref<void(int) const>, const Q&> );

static_assert( is_nothrow_constructible_v<function_ref<void(int)>,
					  constant_wrapper<Q{}>> );
static_assert( is_nothrow_constructible_v<function_ref<void(int) const>,
					  constant_wrapper<Q{}>> );
static_assert( is_nothrow_constructible_v<function_ref<void()>,
					  constant_wrapper<Q{}>, int&> );
static_assert( is_nothrow_constructible_v<function_ref<void() const>,
					  constant_wrapper<Q{}>, int&> );
static_assert( ! is_constructible_v<function_ref<void()>,
				    constant_wrapper<Q{}>, int> );
static_assert( ! is_constructible_v<function_ref<void() const>,
				    constant_wrapper<Q{}>, int> );

static_assert( is_nothrow_constructible_v<function_ref<void()>,
					  constant_wrapper<Q{}>, int*> );
static_assert( ! is_constructible_v<function_ref<void() const>,
				    constant_wrapper<Q{}>, int*> );

struct L
{
  void operator()() &;
};

static_assert( is_nothrow_constructible_v<function_ref<void()>, L> );
static_assert( is_nothrow_constructible_v<function_ref<void()>, L&> );
static_assert( ! is_constructible_v<function_ref<void()>, const L&> );
static_assert( ! is_constructible_v<function_ref<void() const>, L> );
static_assert( ! is_constructible_v<function_ref<void() const>, const L&> );
static_assert( ! is_constructible_v<function_ref<void()>,
				    constant_wrapper<L{}>> );
static_assert( ! is_constructible_v<function_ref<void() const>,
				    constant_wrapper<L{}>> );

struct R
{
  void operator()(float) const&&;
};

static_assert( ! is_constructible_v<function_ref<void(float)>, R> );
static_assert( ! is_constructible_v<function_ref<void(float)>, R&> );
static_assert( ! is_constructible_v<function_ref<void(float) const>, R> );
static_assert( ! is_constructible_v<function_ref<void(float) const>, R&> );
static_assert( ! is_constructible_v<function_ref<void(float) const>, const R&> );

static_assert( ! is_constructible_v<function_ref<void(float)>,
				    constant_wrapper<R{}>> );
static_assert( ! is_constructible_v<function_ref<void(float) const>,
				    constant_wrapper<R{}>> );

constexpr bool
test_constexpr()
{
  function_ref<void(S)> fp1(std::cw<funS>);
  function_ref<void(S)> fp3(std::cw<&funS>);
  function_ref<void(S)> fp4(std::cw<&S::x>);
  function_ref<void(S)> fp5(std::cw<&S::f>);

  S s;
  function_ref<void()> fp6(std::cw<&funS>, s);
  function_ref<void()> fp7(std::cw<&S::x>, s);
  function_ref<void()> fp8(std::cw<&S::x>, &s);
  function_ref<void()> fp9(std::cw<&S::f>, s);
  function_ref<void()> fp10(std::cw<&S::f>, &s);

  M m;
  function_ref<void()> fm1(m);
  function_ref<void()> fm2(std::move(m));

  Q q;
  constexpr Q cq;
  function_ref<void(int)> fq1(q);
  function_ref<void(int) const> fq2(q);
  function_ref<void(int) const> fq3(std::move(q));

  function_ref<void(int)> fcq1(cq);
  function_ref<void(int) const> f(cq);
  function_ref<void(int)> fcq3(std::cw<cq>);
  function_ref<void(int) const> fcq4(std::cw<cq>);

  int i = 0;
  function_ref<void()> fcq5(std::cw<cq>, i);
  function_ref<void() const> fcq6(std::cw<cq>, i);
  function_ref<void()> fcq7(std::cw<cq>, &i);

  L l;
  function_ref<void()> fl1(l);
  function_ref<void()> fl2(std::move(l));

  return true;
}
static_assert( test_constexpr() );

void func();

void
test_instantiation()
{
  function_ref<void(S)> fp1(funS);
  function_ref<void(S)> fp2(&funS);

  test_constexpr();
}
