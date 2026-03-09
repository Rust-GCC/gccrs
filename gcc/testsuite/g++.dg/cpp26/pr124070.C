// PR c++/124070
// { dg-do compile { target c++26 } }

constexpr decltype (nullptr)
bar (bool x)
{
  if (x)
    throw 1;
  return nullptr;
}

template <decltype (nullptr) I = bar (true)>	// { dg-error "uncaught exception '1'" }
constexpr void
foo ()
{
}

consteval {
  foo <> ();					// { dg-error "no matching function for call to 'foo<>\\\(\\\)'" }
}
