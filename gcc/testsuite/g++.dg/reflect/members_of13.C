// PR c++/124831
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

namespace N
{
  enum { A, B, C };
  enum { D };
  enum { E, F, G, H, I };
}

namespace O
{
  typedef enum { A, B, C } J;
  typedef enum { D } K;
  typedef enum { E, F, G, H, I } L;
}

static_assert (members_of (^^N, std::meta::access_context::current ()).size () == 3);
static_assert (enumerators_of (members_of (^^N, std::meta::access_context::current ())[0])[0] == ^^N::A
	       || enumerators_of (members_of (^^N, std::meta::access_context::current ())[1])[0] == ^^N::A
	       || enumerators_of (members_of (^^N, std::meta::access_context::current ())[2])[0] == ^^N::A);
static_assert (enumerators_of (members_of (^^N, std::meta::access_context::current ())[0])[0] == ^^N::D
	       || enumerators_of (members_of (^^N, std::meta::access_context::current ())[1])[0] == ^^N::D
	       || enumerators_of (members_of (^^N, std::meta::access_context::current ())[2])[0] == ^^N::D);
static_assert (enumerators_of (members_of (^^N, std::meta::access_context::current ())[0])[0] == ^^N::E
	       || enumerators_of (members_of (^^N, std::meta::access_context::current ())[1])[0] == ^^N::E
	       || enumerators_of (members_of (^^N, std::meta::access_context::current ())[2])[0] == ^^N::E);
static_assert (members_of (^^O, std::meta::access_context::current ()).size () == 6);
