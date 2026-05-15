// PR c++/125179
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#define A(x, y) \
  struct S##x##0 { S##y##9 *a; }; \
  struct S##x##1 { S##x##0 *a; }; \
  struct S##x##2 { S##x##1 *a; }; \
  struct S##x##3 { S##x##2 *a; }; \
  struct S##x##4 { S##x##3 *a; }; \
  struct S##x##5 { S##x##4 *a; }; \
  struct S##x##6 { S##x##5 *a; }; \
  struct S##x##7 { S##x##6 *a; }; \
  struct S##x##8 { S##x##7 *a; }; \
  struct S##x##9 { S##x##8 *a; }
#define B(x, y) \
  A (x##0, y##9); \
  A (x##1, x##0); \
  A (x##2, x##1); \
  A (x##3, x##2); \
  A (x##4, x##3); \
  A (x##5, x##4); \
  A (x##6, x##5); \
  A (x##7, x##6); \
  A (x##8, x##7); \
  A (x##9, x##8)
#define C(x, y) \
  B (x##0, y##9); \
  B (x##1, x##0); \
  B (x##2, x##1); \
  B (x##3, x##2); \
  B (x##4, x##3); \
  B (x##5, x##4); \
  B (x##6, x##5); \
  B (x##7, x##6); \
  B (x##8, x##7); \
  B (x##9, x##8)
#define D(x, y) \
  C (x##0, y##9); \
  C (x##1, x##0); \
  C (x##2, x##1); \
  C (x##3, x##2); \
  C (x##4, x##3); \
  C (x##5, x##4); \
  C (x##6, x##5); \
  C (x##7, x##6); \
  C (x##8, x##7); \
  C (x##9, x##8)
struct S09998;
struct S09999;
D (1, 0);
D (2, 1);
D (3, 2);
D (4, 3);
struct S09999 { S49999 *a; S09998 *b; };
S49999 a = { nullptr };
struct S09998 { decltype (^^::) a; };
constexpr S49999 b = { nullptr };
