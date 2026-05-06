// PR c++/107797
// { dg-do compile { target c++11 } }
// { dg-options "-Wunused" }

typedef __INTPTR_TYPE__ intptr_t;

void
g ()
{
  (intptr_t) new int{};
  intptr_t(new int{});
  (intptr_t) new int();
  intptr_t(new int());
}
