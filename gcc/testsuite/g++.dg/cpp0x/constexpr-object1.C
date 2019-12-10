// { dg-do compile { target c++11 } }

// From N2235

// 4.5.2 semantics

// p 1 constexpr specifier
// objects, static const data
struct A1 { int i; };	   // { dg-message "no user-provided default constructor" }

constexpr int i1 = 1024;
constexpr A1 a1 = A1();

// error: not a definition
extern constexpr int i2; // { dg-error "definition" }

// error: missing initializer
constexpr A1 a2; // { dg-error "uninitialized 'const" }

const constexpr A1 a3 = A1();

volatile constexpr A1 a4 = A1(); // { dg-bogus "both .volatile. and .constexpr. cannot" }

// error: on type declaration
constexpr struct pixel // { dg-error "cannot be used for type declarations" }
{
  int x;
  int y;
};
