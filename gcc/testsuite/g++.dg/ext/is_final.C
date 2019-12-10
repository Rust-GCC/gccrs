// PR c++/51365
// { dg-do compile { target c++11 } }

struct A { };
static_assert( ! __is_final (A), "A not final" );

struct Af final { };
static_assert( __is_final (Af), "Af is final" );

class B { };
static_assert( ! __is_final (B), "B not final" );

class Bf final { };
static_assert( __is_final (Bf), "Bf is final" );

struct C : private A, private B { };
static_assert( ! __is_final (C), "C not final" );

struct Cf final : private A, private B { };
static_assert( __is_final (Cf), "Cf is final" );

struct D { virtual ~D() final { } };
static_assert( ! __is_final (D), "D not final" );

struct Df final { virtual ~Df() final { } };
static_assert( __is_final (Df), "Df is final" );

template<typename> struct E { };
static_assert( ! __is_final (E<int>), "E<int> not final" );
static_assert( ! __is_final (E<Af>),  "E<Af> not final" );

template<typename> struct Ef final { };
static_assert( __is_final (Ef<int>), "Ef<int> is final" );
static_assert( __is_final (Ef<A>),   "Ef<A> is final" );
static_assert( __is_final (Ef<Af>),  "Ef<Af> is final" );

template<typename> struct F { virtual ~F() final { }; };
static_assert( ! __is_final (F<int>), "F<int> not final" );
static_assert( ! __is_final (F<Af>),  "F<Af> not final" );

template<typename> struct Ff final { virtual ~Ff() final { }; };
static_assert( __is_final (Ff<int>), "Ff<int> is final" );
static_assert( __is_final (Ff<A>),   "Ff<A> is final" );
static_assert( __is_final (Ff<Af>),  "Ff<Af> is final" );

// PR 85254

template <class T> struct final_trait_wrap{ typedef T type; };

template <class T> struct my_is_final
{
  static const bool value = __is_final(typename final_trait_wrap<T>::type);
};

struct final1 final {};
template <typename T> struct final2 final {};

static_assert( my_is_final<final1>::value, "final1 is final" );
static_assert( my_is_final<final2<int>>::value, "final2<int> is final" );
