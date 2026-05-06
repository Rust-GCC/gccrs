// PR c++/123889
// { dg-do compile { target c++11 } }

template <class T>
constexpr int func (T) {
// { dg-message "'constexpr int func\\\(T\\\) \\\[with T = int\\\]' is not usable as a 'constexpr' function because:" "" { target c++11_only } .-1 }
// { dg-error "body of 'constexpr' function 'constexpr int func\\\(T\\\) \\\[with T = int\\\]' not a return-statement" "" { target c++11_only } .-2 }
  return 1;
  return 2;
}

static_assert (func (1) == 1, "");
// { dg-error "non-constant condition for static assertion" "" { target c++11_only } .-1 }
// { dg-error "'constexpr int func\\\(T\\\) \\\[with T = int\\\]' called in a constant expression" "" { target c++11_only } .-2 }
