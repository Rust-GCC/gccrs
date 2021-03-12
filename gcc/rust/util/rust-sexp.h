/* Serialize your objects into S-expression.
   Implement SexpSerializable on your object, then call sexp() as needed. */

#ifndef RUST_SEXP_H
#define RUST_SEXP_H

#include <string>
#include <type_traits>

namespace Rust {

class SexpSerializable
{
public:
  /* Serialize this object to S-expression. */
  virtual std::string to_sexp () const = 0;
};

inline std::string
to_sexp (const std::string &str)
{
  return str;
}

inline std::string
to_sexp (const char *str)
{
  return std::string (str);
}

inline std::string
to_sexp (const SexpSerializable &obj)
{
  return obj.to_sexp ();
}

inline std::string
to_sexp (std::nullptr_t ptr)
{
  return "[nullptr]";
}

template <typename T> struct helper
{
  using type = void *;
};

template <typename T> using helper_t = typename helper<T>::type;

template <typename T,
	  typename std::enable_if<std::is_pointer<T>::value, void *>::type
	  = nullptr>
std::string
to_sexp (const T &ptr)
{
  return to_sexp (*ptr);
}

template <typename T, typename Rust::helper_t<decltype (
			std::declval<T> ().T::operator* ())> = nullptr>
std::string
to_sexp (const T &ptr)
{
  return to_sexp (*ptr);
}

template <typename T, typename Rust::helper_t<
			decltype (std::declval<T> ().cbegin ())> = nullptr>
std::string
to_sexp (const T &container)
{
  std::string str;
  for (const auto &item : container)
    {
      str += " ";
      str += to_sexp (item);
    }
  str.erase (str.begin ());
  return str;
}

template <typename... Ts>
std::string
sexp_inner ()
{
  return "";
}

template <typename... Ts, typename U>
std::string
sexp_inner (const U &stuff, const Ts &...rest)
{
  return " " + to_sexp (stuff) + sexp_inner (rest...);
}

/* Creates a compound S-expression.
   Accepts an arbitary number of parameters.
   The following objects are serializable:
     - std::string
     - any class that implements SexpSerializable
     - containers of serializable objects(e.g. std::vector)
     - pointers to serializable objects(e.g. std::unique_ptr)
 */
template <typename... Ts>
std::string
sexp (const Ts &...rest)
{
  std::string str = sexp_inner (rest...);
  // Remove the first character, which is an extra whitespace
  str.erase (str.begin ());
  return "(" + str + ")";
}

} // namespace Rust

#endif
