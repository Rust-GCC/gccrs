/* Serialize your objects into S-expression.
   Implement SexpSerializable on your object, then call sexp() as needed. */

#include <string>

class SexpSerializable
{
public:
  /* Serialize this object to S-expression. */
  virtual std::string to_sexp () const = 0;
};

template <class... Ts>
static std::string
sexp_inner ()
{
  return "";
}

// Accept a string
template <class... Ts>
static std::string
sexp_inner (const std::string &str, Ts... rest)
{
  return " " + str + sexp_inner (rest...);
}

// Accept a SexpSerializable
template <class... Ts>
static std::string
sexp_inner (const SexpSerializable &obj, Ts... rest)
{
  return " " + obj.to_sexp ();
}

// Accept a container. Calls to_sexp() on all its objects and concatenates the
// results.
template <class U, class... Ts, class = decltype (std::declval<U> ().cbegin ())>
static std::string
sexp_inner (const U &container, Ts... rest)
{
  std::string str;
  for (const auto &item : container)
    {
      str += " ";
      str += item.to_sexp ();
    }
  return str;
}

/* Creates a compound S-expression.
   Accepts an arbitary number of parameters.
   Parameters can be string, SexpSerializeable, or containers of
   SexpSerializable. */
template <class... Ts>
std::string
sexp (Ts... rest)
{
  std::string str = sexp_inner (rest...);
  // Remove the first character, which is an extra whitespace
  str.erase (str.begin ());
  return "(" + str + ")";
}
