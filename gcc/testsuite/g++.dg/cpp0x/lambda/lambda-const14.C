// PR c++/123536
// { dg-do compile { target c++11 } }

template<class T>
void
f ()
{
  int& c = T::x;	// { dg-error ".x. is not a member of .int." }
  auto l = [] { c++; };	// { dg-error ".c. is not captured" }
}

void
g ()
{
  f<int>();
}
