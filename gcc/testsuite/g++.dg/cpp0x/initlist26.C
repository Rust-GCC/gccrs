// PR c++/42059
// { dg-do compile { target c++11 } }
// { dg-options "" }

void
foo (int i)
{
  int a[i];
  a = { }; // { dg-error "5:assigning to an array from an initializer list" }
}
