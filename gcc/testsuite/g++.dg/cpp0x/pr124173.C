// PR c++/124173
// { dg-do compile { target c++11 } }

template <decltype (nullptr)>
void
foo ()
{
}

int
main ()
{
  constexpr decltype (nullptr) t = nullptr;
  foo <t> ();
}
