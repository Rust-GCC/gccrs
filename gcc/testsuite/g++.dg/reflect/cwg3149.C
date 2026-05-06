// CWG3149 - Rvalues in destructuring expansion statements
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>
#include <tuple>

std::tuple <int, long long &, int &&> foo ();

void
bar ()
{
  template for (auto &&elem : foo ())
    static_assert (remove_reference (type_of (^^elem)) == ^^long long
		   ? type_of (^^elem) == ^^long long &
		   : type_of (^^elem) == ^^int &&);
}
