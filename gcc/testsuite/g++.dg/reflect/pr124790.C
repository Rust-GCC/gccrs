// PR c++/124790
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection -Wunused-but-set-variable -Wunused-but-set-parameter" }

consteval auto
foo ()
{
  int a = 42;		// { dg-bogus "variable 'a' set but not used" }
  return ^^a;
}

consteval auto
bar ()
{
  int a = 42;		// { dg-bogus "variable 'a' set but not used" }
  a = 7;
  a = 9;
  return ^^a;
}

consteval auto
baz (int a)		// { dg-bogus "parameter 'a' set but not used" }
{
  return ^^a;
}

consteval auto
qux (int a)		// { dg-bogus "parameter 'a' set but not used" }
{
  a = 7;
  a = 9;
  return ^^a;
}

constexpr auto a = foo ();
constexpr auto b = bar ();
constexpr auto c = baz (42);
constexpr auto d = qux (42);
