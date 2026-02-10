// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

int x = 42;
template<typename T>
constexpr T two = 2;

template<typename T>
constexpr T foo (T t) { return t; }

void bar () { }

struct S { };

template<typename>
struct ST { };

template <int P1, const int &P2> void fn() {}

static constexpr int p[2] = {1, 2};
constexpr auto spec = ^^fn<p[0], p[1]>;

void
g ()
{
  int i1 = [: ^^x :];
  int i2 = template [: ^^x :];    // { dg-error "expected a reflection of a function template" }
  // { dg-message "but .x. is a variable" "" { target *-*-* } 4 }
  int i3 = [: ^^two<int> :];
  int i4 = template [: ^^two<int> :]; // { dg-error "expected a reflection of a function template" }
  // { dg-message "but .two<int>. is a variable" "" { target *-*-* } 6 }
  int i5 = [: ^^foo :](42);	      // { dg-error "expected 'template' keyword before dependent template name" }
  int i6 = template [: ^^foo :](42);
  int i7 = [: ^^foo<int> :](42);
  int i8 = template [: ^^foo<int> :](42);   // { dg-error "expected a reflection of a function template" }
  // { dg-message "but .foo<int>. is a function" "" { target *-*-* } 9 }
  int i9 = [: ^^foo :]<int>(42);	    // { dg-error "expected .template. keyword before dependent template name" }
  int i10 = template [: ^^foo :]<int>(42);
  int i11 = template [: ^^bar :]<int>(42);  // { dg-error "no matching function for call" }
  int i12 = [: ^^two :]<int>;		    // { dg-error "expected 'template' keyword before dependent template name" }
  int i13 = template [: ^^two :]<int>;

  [: ^^ST :]<int> c1;  // { dg-error "expected a reflection of an expression" }
  // { dg-message "but .ST<int>. is a type" "" { target *-*-* } 16 }
  typename [: ^^ST :]<int> c2;
  template [: ^^ST :]<int> c3;  // { dg-error "expected a reflection of an expression" }
  [: ^^S :]<int> c4;   // { dg-error "expected a reflection of an expression|expected primary-expression" }
  // { dg-message "but .S. is a type" "" { target *-*-* } 13 }
  // { dg-message "add .typename. to denote a type outside a type-only context" "" { target *-*-* } .-2 }
  template [: ^^S :]<int> c5;   // { dg-error ".S. is not a template" }
  typename [: ^^S :]<int> c6;   // { dg-error ".S. is not a template|expected" }
  [: ^^bar :]<int>();	// { dg-error "expected" }

  auto x1 = [: ^^ST :]<int>{};	  // { dg-error "expected a reflection of an expression" }
  auto x2 = template [: ^^ST :]<int>{};	// { dg-error "expected a reflection of an expression" }
  auto x3 = typename [: ^^ST :]<int>{};
}
