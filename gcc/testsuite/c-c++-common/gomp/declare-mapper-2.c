/* Eventually: Check that strided use of 'declare mapper' is rejected for
   'target enter data' but accepted for 'target update'

   However, it currently fails already for parsing. */

struct t
{
  int n;
  int *p;
};

#pragma omp declare mapper(my : struct t s) map(to: s, s.p[0:s.n:2])
// { dg-error "65: expected '\\\]' before ':' token" "" { target c } .-1 }
// { dg-error "65: expected '\\]' before ':' token" "" { target c++ } .-2 }
// { dg-error "65: expected '\\)' before ':' token" "" { target c++ } .-3 }

// FIXME: ensure that [:s.n:2] also parses in C++26 despite [: ... :] syntax.
// FIXME: remove ifndef:
#ifndef __cplusplus  // Silence some more errors until strided is implemented
void f() {
  struct t v;
  #pragma omp target enter data map(mapper(my), to: v)
  #pragma omp target update to(mapper(my): v)
}
#endif
