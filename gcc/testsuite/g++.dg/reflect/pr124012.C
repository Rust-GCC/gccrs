// PR c++/124012
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

void foo (char);
void corge (const char *);
struct A { char a; decltype (^^::) b; };

void
bar ()
{
  constexpr auto [a, b] = A {};
  foo (a);			// { dg-error "consteval-only expressions are only allowed in a constant-evaluated context" }
}

void
baz ()
{
  constexpr auto a = A {};
  foo (a.a);			// { dg-error "consteval-only expressions are only allowed in a constant-evaluated context" }
}

void
qux ()
{
  constexpr auto a = A {};
  corge (&a.a);			// { dg-error "consteval-only expressions are only allowed in a constant-evaluated context" }
}

void
garply ()
{
  constexpr auto [a, b] = A {};
  corge (&a);			// { dg-error "consteval-only expressions are only allowed in a constant-evaluated context" }
}

void
fred ()
{
  constexpr auto [a, b] = A {};
  constexpr auto c = a;
  foo (c);
  corge (&c);
}
