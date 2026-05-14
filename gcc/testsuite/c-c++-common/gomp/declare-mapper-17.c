typedef struct S { int x;} S;
int x;
S s;

void f() {
  #pragma omp declare mapper(S var) map(to: x) /* { dg-error "at least one 'map' clause must map 'var' or an element of it" } */
  #pragma omp target enter data map(s)
}

void f2()
{
  int x;
  #pragma omp declare mapper (int var) map(always, to: var)
// { dg-error "'int' is not a struct or union type in '#pragma omp declare mapper'" "" { target c } .-1 }
// { dg-error "'int' is not a struct, union or class type in '#pragma omp declare mapper'" "" { target c++ } .-2 }
  #pragma omp target enter data map(x)
}

void f3() {
  float g[2];
  #pragma omp declare mapper (S var) map(always, to: g[var.x]) // { dg-error "at least one 'map' clause must map 'var' or an element of it" }
  #pragma omp target enter data map(g)
}
