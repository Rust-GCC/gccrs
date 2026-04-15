// CWG3149 - Rvalues in destructuring expansion statements
// { dg-do compile { target c++26 } }

#include <tuple>
#include <type_traits>

std::tuple <int, long long &, int &&> foo ();

void
bar ()
{
  template for (auto &&elem : foo ())
    static_assert (std::is_same_v <std::remove_reference_t <decltype (elem)>, long long>
		   ? std::is_same_v <decltype (elem), long long &>
		   : std::is_same_v <decltype (elem), int &&>);
}
