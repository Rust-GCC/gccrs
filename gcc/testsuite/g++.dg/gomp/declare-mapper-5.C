// { dg-do compile { target c++11 } }

struct S { int x; };

template<typename T>
void f()
{
  T x;
  int y;
  #pragma omp declare mapper (T var) map(always, to: var, y) // { dg-error "'int' is not a struct, union or class type in '#pragma omp declare mapper'" }
  #pragma omp target enter data map(x)
}

void g()
{
  f<int>();
}

template<typename T>
void h() {
  T g[2];
  #pragma omp declare mapper (S var) map(always, to: g[var.x]) // { dg-error "at least one 'map' clause must map 'var' or an element of it" }
  #pragma omp target enter data map(g)
}

void h2()
{
  h<S>();
}
