// { dg-do compile { target c++26 } }
// { dg-options "" }
// { dg-additional-options "-freflection" }
// Test that we offer some helpful diagnostic.

struct S {
  template<typename T>
  void tfn (T) { }
};

template<typename T>
constexpr T fortytwo = 42;

void
f ()
{
  S s;
  s.[: ^^S::tfn :](42); // { dg-warning "expected 'template' keyword before dependent template name" }
  s.template [: ^^S::tfn :](42);

  constexpr auto r = ^^fortytwo;
  constexpr int i1 = [:r:]<int>; // { dg-warning "expected 'template' keyword before dependent template name" }
  constexpr int i2 = template [:r:]<int>;
}
