// PR c++/125454
// { dg-do compile { target c++11 } }

template <typename T>
void
foo()
{
  T a = a[a]; // { dg-error "array subscript is not an integer" }
}

void
bar ()
{
  foo<int[]>;
}
