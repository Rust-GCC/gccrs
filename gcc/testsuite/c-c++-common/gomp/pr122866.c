/* { dg-do "compile" } */

struct test { int x; };

#pragma omp begin declare target
void froggify (struct test);
#pragma omp end declare target

#pragma omp declare mapper(struct test v) map(iterator(i = 0:1), tofrom: v.x)
/* { dg-message "sorry, unimplemented: user-defined mapper that uses a .map. clause with .iterator." "" { target *-*-* } .-1 } */
/* { dg-message "'#pragma omp declare mapper' with '-std=' set to before C++11" "" { target c++98_only } .-2 } */

int
main ()
{
  struct test s;
  s.x = -1;
  #pragma omp target
  {
    froggify (s);
  }
}
