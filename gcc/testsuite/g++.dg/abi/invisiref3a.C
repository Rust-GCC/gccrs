// PR c++/107187
// { dg-do compile { target c++11 } }
// { dg-additional-options "-Wabi=10 -fdump-tree-gimple" }
// { dg-final { scan-tree-dump "struct S &" "gimple" } }

struct S
{
  ~S ();
  int i;
};

int
foo (S s)
{
  return s.i;
}

S getS ();

void
bar ()
{
  foo (getS ());
}

static_assert (!__is_trivially_copyable (S), "");
