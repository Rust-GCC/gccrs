// { dg-do compile { target { c || c++11 } } }
// { dg-additional-options "-fdump-tree-gimple" }

typedef struct S { int x;} S;
int x;
S s1;
S s2;

void f() {
  #pragma omp declare mapper(S var) map(to: var)
  #pragma omp target exit data map(always, from: s1)
}

void f2() {
  #pragma omp declare mapper(S var) map(present, to: var)
  #pragma omp target enter data map(always, tofrom : s2)
}

void f3()
{
  S x;
  int y;
  #pragma omp declare mapper (S var) map(always, to: var, y)
  #pragma omp target enter data map(x)
}


// to + 'exit date' always from -> release
// { dg final { scan-dump "static int omp declare mapper <default> = struct S #pragma omp declare mapper \\(struct S var\\) map\\(to:var\\);" "gimple" }
// { dg final { scan-dump "#pragma omp target exit data map\\(release:s1 \\\[len: 4\\\]\\)" "gimple" }


// present,to + always,from -> always,present,to
// { dg final { scan-dump "static int omp declare mapper <default> = struct S #pragma omp declare mapper \\(struct S var\\) map\\(present,to:var\\);" "gimple" }
// { dg final { scan-dump "#pragma omp target enter data map\\(always,present,to:s2 \\\[len: 4\\\]\\)" "gimple" }

// always,to + - -> always,to
// { dg final { scan-dump "static int omp declare mapper <default> = struct S #pragma omp declare mapper \\(struct S var\\) map\\(always,to:y\\) map\\(always,to:var\\);" "gimple" }
// { dg final { scan-dump "#pragma omp target enter data map\\(always,to:y \\\[len: 4\\\]\\) map\\(always,to:x \\\[len: 4\\\]\\)" "gimple" }
