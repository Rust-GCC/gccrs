// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }
// Test std::meta::access_context.

#include <meta>

using namespace std::meta;

constexpr auto a = current_function ();		// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a function'" }

consteval info
foo (info x = current_function ())
{
  return x;
}

static_assert (foo (^^std) == ^^std);
constexpr auto b = foo ();			// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a function'" }

namespace N
{
  constexpr auto c = current_function ();	// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a function'" }
  constexpr auto d = foo ();			// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a function'" }
  namespace M
  {
    constexpr auto e = current_function ();	// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a function'" }
    constexpr auto f = foo ();			// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a function'" }
    consteval {
      constexpr auto g = current_function ();	// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a function'" }
      consteval {
	constexpr auto h = foo ();		// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a function'" }
      }
    }
  }
}

struct T
{
  static_assert (current_function () != ^^T);	// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a function'" }
						// { dg-error "non-constant condition for static assertion" "" { target *-*-* } .-1 }
  static_assert (foo () != ^^T);		// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a function'" }
  info t = current_function ();			// { dg-error "non-constant condition for static assertion" "" { target *-*-* } .-1 }
};

struct U
{
  consteval U () {}
  info u = current_function ();
};

struct V : U
{
  using U::U;
  info v = current_function ();
  consteval {
    static_assert (current_function () != ^^V);		// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a function'" }
    consteval {						// { dg-error "non-constant condition for static assertion" "" { target *-*-* } .-1 }
      static_assert (current_function () != ^^V);	// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a function'" }
    }							// { dg-error "non-constant condition for static assertion" "" { target *-*-* } .-1 }
  }
};

consteval {
  static_assert (current_function () != ^^::);		// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a function'" }
  consteval {						// { dg-error "non-constant condition for static assertion" "" { target *-*-* } .-1 }
    static_assert (current_function () != ^^::);	// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a function'" }
  }							// { dg-error "non-constant condition for static assertion" "" { target *-*-* } .-1 }
}

namespace O
{
  int a[2];
  auto
  foo () -> int (&) [current_function () != ^^O ? 2 : 3]	// { dg-error "size of array is not an integral constant-expression" }
  {
    return a;						// { dg-error "invalid initialization of reference of type" }
  }
}
