// PR c++/107187
// { dg-do compile { target c++11 } }
// { dg-additional-options "-fdump-tree-gimple" }
// { dg-final { scan-tree-dump-not "struct S &" "gimple" } }
// { dg-final { scan-tree-dump "S::~S" "gimple" } }

struct __attribute__ ((__trivial_abi__)) S
{
  S () {}
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
