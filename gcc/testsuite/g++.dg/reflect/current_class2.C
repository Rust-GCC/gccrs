// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }
// Test std::meta::current_class.

#include <meta>

using namespace std::meta;

constexpr auto a = current_class ();		// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a class nor a member function'" }

consteval info
foo (info x = current_class ())
{
  return x;
}

void
qux ()
{
  static constexpr auto a = current_class ();	// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a class nor a member function'" }
}

static_assert (foo (^^std) == ^^std);
constexpr auto b = foo ();			// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a class nor a member function'" }

namespace N
{
  struct S {};
  constexpr auto c = foo ();			// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a class nor a member function'" }
  namespace M
  {
    constexpr auto d = current_class ();	// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a class nor a member function'" }
    consteval {
      auto e = foo ();
      consteval {
	auto f = current_class ();
      }						// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a class nor a member function'" }
    }						// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a class nor a member function'" }
  }
}

struct T
{
  info t = current_class ();
};

void
bar ()
{
  static constexpr auto g = current_class ();	// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a class nor a member function'" }
  static constexpr auto h = T{}.t;		// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a class nor a member function'" }
  auto l2 = [] () -> int (&) [current_class () != ^^bar ? 2 : 3] {	// { dg-error "size of array is not an integral constant-expression" }
    static int a[2];
    return a;					// { dg-error "invalid initialization of reference of type" }
  };
}

consteval {
  auto a = current_class ();
  consteval {
    auto b = foo ();
  }						// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a class nor a member function'" }
}						// { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'current scope does not represent a class nor a member function'" }

namespace O
{
  int a[2];
  auto
  foo () -> int (&) [current_class () != ^^O ? 2 : 3]	// { dg-error "size of array is not an integral constant-expression" }
  {
    return a;						// { dg-error "invalid initialization of reference of type" }
  }
}
