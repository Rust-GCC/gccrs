// LWG4556 - Unclear properties of reflection strings
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

template <const char *P>
void foo () {}

template <const char8_t *P>
void bar () {}

void
baz ()
{
  foo <symbol_of (std::meta::op_plus_equals).data ()> ();	// { dg-error "no matching function for call to" }
  // { dg-error "'\\\&\\\"\\\+=\\\"\\\[0\\\]' is not a valid template argument of type 'const char\\\*'" "" { target *-*-* } .-1 }
  bar <u8symbol_of (std::meta::op_pipe).data ()> ();		// { dg-error "no matching function for call to" }
  // { dg-error "'\\\&\\\"\\\|\\\"\\\[0\\\]' is not a valid template argument of type 'const char8_t\\\*'" "" { target *-*-* } .-1 }
  foo <symbol_of (std::meta::op_plus_equals).data () + 1> ();	// { dg-error "no matching function for call to" }
  // { dg-error "'\\\&\\\"\\\+=\\\"\\\[1\\\]' is not a valid template argument of type 'const char\\\*'" "" { target *-*-* } .-1 }
  bar <u8symbol_of (std::meta::op_pipe).data () + 1> ();	// { dg-error "no matching function for call to" }
  // { dg-error "'\\\&\\\"\\\|\\\"\\\[1\\\]' is not a valid template argument of type 'const char8_t\\\*'" "" { target *-*-* } .-1 }
  foo <identifier_of (^^baz).data ()> ();			// { dg-error "no matching function for call to" }
  // { dg-error "'\\\&\\\"baz\\\"\\\[0\\\]' is not a valid template argument of type 'const char\\\*'" "" { target *-*-* } .-1 }
  bar <u8identifier_of (^^baz).data ()> ();			// { dg-error "no matching function for call to" }
  // { dg-error "'\\\&\\\"baz\\\"\\\[0\\\]' is not a valid template argument of type 'const char8_t\\\*'" "" { target *-*-* } .-1 }
  foo <identifier_of (^^baz).data () + 1> ();			// { dg-error "no matching function for call to" }
  // { dg-error "'\\\&\\\"baz\\\"\\\[1\\\]' is not a valid template argument of type 'const char\\\*'" "" { target *-*-* } .-1 }
  bar <u8identifier_of (^^baz).data () + 1> ();			// { dg-error "no matching function for call to" }
  // { dg-error "'\\\&\\\"baz\\\"\\\[1\\\]' is not a valid template argument of type 'const char8_t\\\*'" "" { target *-*-* } .-1 }
  foo <display_string_of (^^baz).data ()> ();			// { dg-error "no matching function for call to" }
  // { dg-error "'\\\&\\\"void baz\\\(\\\)\\\"\\\[0\\\]' is not a valid template argument of type 'const char\\\*'" "" { target *-*-* } .-1 }
  bar <u8display_string_of (^^baz).data ()> ();			// { dg-error "no matching function for call to" }
  // { dg-error "'\\\&\\\"void baz\\\(\\\)\\\"\\\[0\\\]' is not a valid template argument of type 'const char8_t\\\*'" "" { target *-*-* } .-1 }
  foo <display_string_of (^^baz).data () + 1> ();		// { dg-error "no matching function for call to" }
  // { dg-error "'\\\&\\\"void baz\\\(\\\)\\\"\\\[1\\\]' is not a valid template argument of type 'const char\\\*'" "" { target *-*-* } .-1 }
  bar <u8display_string_of (^^baz).data () + 1> ();		// { dg-error "no matching function for call to" }
  // { dg-error "'\\\&\\\"void baz\\\(\\\)\\\"\\\[1\\\]' is not a valid template argument of type 'const char8_t\\\*'" "" { target *-*-* } .-1 }
}
