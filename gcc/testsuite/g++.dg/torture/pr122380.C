// { dg-do run }

#include <variant>
#include <vector>

using A = std::vector <double>;
using B = std::vector <int>;

struct C
{
  bool c;
  std::variant <A, B> d;
};

[[gnu::noipa, gnu::optimize ("assume-sane-operators-new-delete")]] bool
foo (C *m, A *e)
{
  bool c = false;

  if (m->c)
    {
      *e = std::move (std::get <A> (m->d));
      c = true;
    }
  else
    std::get <B> (m->d);

  delete m;
  return c;
}

int
main ()
{
  A e { 1 };
  C *m = new C;
  m->c = true;
  m->d = A { 1 };
  foo (m, &e);
}
