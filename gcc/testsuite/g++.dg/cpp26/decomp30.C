// CWG3135 - constexpr structured bindings with prvalues from tuples
// { dg-do compile { target c++14 } }
// { dg-options "" }

namespace std {
  template <typename T> struct tuple_size;
  template <int, typename> struct tuple_element;
}

struct A {
  template <int I> constexpr int get () const { return I + 6; }
};

template <> struct std::tuple_size <A> { static const int value = 2; };
template <int I> struct std::tuple_element <I, A> { using type = int; };
template <> struct std::tuple_size <const A> { static const int value = 2; };
template <int I> struct std::tuple_element <I, const A> { using type = const int; };

constexpr A
bar ()
{
  return A {};
}

template <int N>
constexpr int
foo ()
{
  constexpr auto [...i] = bar ();	// { dg-warning "structured binding packs only available with" "" { target { c++17 && c++23_down } } }
					// { dg-warning "structured binding declaration can be 'constexpr' only with" "" { target c++23_down } .-1 }
					// { dg-warning "structured bindings only available with" "" { target c++14_down } .-2 }
  return i...[0] + i...[1];		// { dg-warning "pack indexing only available with" "" { target c++23_down } }
}

static_assert (foo <42> () == 13);
