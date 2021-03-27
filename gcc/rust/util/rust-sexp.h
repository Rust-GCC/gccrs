#include <algorithm>
#include <initializer_list>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "rust-inline-visitor.h"
#include "rust-make-unique.h"
#include "rust-poly.h"

#ifndef RUST_SEXP
#define RUST_SEXP

namespace Rust {

template <typename T> struct helper
{
  using type = std::nullptr_t;
};
template <typename T> using helper_t = typename helper<T>::type;

template <typename T>
static std::string
join (const std::string &sep, const T &items)
{
  std::string result;
  for (auto it = items.cbegin (); it != items.cend (); ++it)
    {
      if (it != items.cbegin ())
	{
	  result += " ";
	}
      result += *it;
    }
  return result;
}

class sexp;

struct SString;
struct SInt;
struct SFloat;
struct SList;

class SValueVisitor
{
public:
  virtual ~SValueVisitor () = default;
  virtual void visit (SString &s) = 0;
  virtual void visit (SInt &s) = 0;
  virtual void visit (SFloat &s) = 0;
  virtual void visit (SList &s) = 0;
};

struct SValue
{
  virtual ~SValue () = default;
  virtual void accept_vis (SValueVisitor &vis) = 0;
};

struct SString : SValue
{
  std::string value;

  SString (const std::string &str) : value (str) {}

  virtual void accept_vis (SValueVisitor &vis) override { vis.visit (*this); }
};

struct SInt : SValue
{
  int64_t value;

  SInt (int64_t value) : value (value) {}

  virtual void accept_vis (SValueVisitor &vis) override { vis.visit (*this); }
};

struct SFloat : SValue
{
  double value;

  SFloat (double value) : value (value) {}

  virtual void accept_vis (SValueVisitor &vis) override { vis.visit (*this); }
};

struct SList : SValue
{
  std::vector<sexp> value;

  template <typename It>
  SList (It begin, It end) : value (std::vector<sexp> (begin, end))
  {}

  virtual void accept_vis (SValueVisitor &vis) override { vis.visit (*this); }
};

class sexp
{
private:
  poly<SValue> _value;

  template <typename T> void check_nullptr (T &ptr)
  {
    rust_assert (ptr != nullptr);
  }

  // C++ doesn't allow late binding to member variables,
  // but poly<SValue> doesn't have an "empty" value by design.
  // Therefore we have to use helper functions to initialize _value within
  // initializer list.
  template <typename T> poly<SValue> svalue_from_ptr (T &&ptr)
  {
    check_nullptr (ptr);
    auto s = sexp (*ptr);
    return std::move (s._value);
  }

  template <typename T> poly<SValue> svalue_from_container (const T &container)
  {
    std::vector<sexp> items;
    for (auto &t : container)
      {
	items.emplace_back (t);
      }
    return Rust::make_poly<SList> (items.begin (), items.end ());
  }

  template <typename T> poly<SValue> svalue_from_custom (const T &t)
  {
    auto s = to_sexp (t);
    return std::move (s._value);
  }

public:
  // Construct from std::string
  sexp (const std::string &str) : _value (Rust::make_poly<SString> (str)) {}

  // Construct from char*
  sexp (const char *str) : _value (Rust::make_poly<SString> (str)) {}

  // Construct from integers
  template <typename T,
	    typename std::enable_if<std::is_integral<T>::value, void *>::type
	    = nullptr>
  sexp (T val) : _value (Rust::make_poly<SInt> (val))
  {}

  // Construct from floating point values
  template <typename T, typename std::enable_if<
			  std::is_floating_point<T>::value, void *>::type
			= nullptr>
  sexp (T val) : _value (Rust::make_poly<SFloat> (val))
  {}

  // Construct from raw pointers to objects
  template <typename T,
	    typename std::enable_if<std::is_pointer<T>::value, void *>::type
	    = nullptr>
  sexp (T ptr) : _value (svalue_from_ptr (ptr))
  {}

  // Construct from smart pointers to objects
  template <typename T, Rust::helper_t<decltype (
			  std::declval<T> ().T::operator* ())> = nullptr>
  sexp (T &&ptr) : _value (svalue_from_ptr (std::move (ptr)))
  {}

  // Construct from a container of objects
  template <typename T,
	    Rust::helper_t<decltype (std::declval<T> ().cbegin ())> = nullptr>
  sexp (const T &container) : _value (svalue_from_container (container))
  {}

  // Construct from a std::initializer_list of sexp
  sexp (std::initializer_list<sexp> items)
    : _value (Rust::make_poly<SList> (items.begin (), items.end ()))
  {}

  // Construct from custom conversion function
  template <typename T,
	    Rust::helper_t<decltype (to_sexp (std::declval<T> ()))> = nullptr>
  sexp (const T &t) : _value (svalue_from_custom (t))
  {}

  // Serialize to string
  std::string to_string () const
  {
    std::string ret;
    auto vis
      = begin_visitor<SValueVisitor> ()
	  .on<SString> ([&ret] (SString &s) { ret = s.value; })
	  .on<SInt> ([&ret] (SInt &s) { ret = std::to_string (s.value); })
	  .on<SFloat> ([&ret] (SFloat &s) { ret = std::to_string (s.value); })
	  .on<SList> ([&ret] (SList &s) {
	    std::vector<std::string> inner;
	    std::transform (s.value.begin (), s.value.end (),
			    std::back_inserter (inner),
			    [] (const sexp &s) { return s.to_string (); });
	    ret += "(";
	    ret += join (" ", inner);
	    ret += ")";
	  })
	  .end_visitor ();
    _value->accept_vis (vis);

    return ret;
  }
};

} // namespace Rust

#endif
