// { dg-do compile { target c++11 } }
// { dg-additional-options "-fdump-tree-original" }

// Ensure that always in mapper does not ICE and gets propagated to both 'x' and 'y'

struct S { int x; };

template<typename T>
void f()
{
  T x;
  int y;
  #pragma omp declare mapper (T var) map(always, to: var, y)
// error: the type 'S' of 'constexpr' variable '#pragma omp declare mapper' is not literal
  #pragma omp target enter data map(x)
}

void g()
{
  f<S>();
}

// { dg-final { scan-tree-dump "#pragma omp target enter data map\\(always,to:y\\) map\\(always,to:x\\)" "original" } }
