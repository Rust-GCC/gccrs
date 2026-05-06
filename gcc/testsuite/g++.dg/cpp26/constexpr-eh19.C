// PR c++/124755
// { dg-do compile { target c++26 } }

constexpr int
foo (int a)
{
  return a ? throw 1 : 0;
}

consteval int
bar ()
{
  int n = 0;
  int a[] = { 0, 1 };
  for (int m : a)
    n += foo (m);
  return n;
}

consteval int
baz ()
{
  try
    {
      return bar ();
    }
  catch (...)
    {
      return -1;
    }
}

static_assert (baz () == -1);
