/* Test that GNU11 warnings are produced about initializers that might
 * not zero padding bits of structs or unions.
 */
/* { dg-do run } */
/* { dg-options "-std=gnu11 -Wzero-init-padding-bits=all" } */

struct A { unsigned char a; long long b; };
struct B { unsigned char a; long long b; struct A c[3]; };
struct C { struct A a; };
struct D { unsigned char a; long long b; struct C c; };
struct E { long long a; long long b; };
union U { unsigned char a; long long b; };
union V { long long a; long long b; };
struct F { long long a; union U b; };

int
main (int argc, char *argv[])
{
  struct A a = {}; /* ZI because it omits a, b */
  struct B b = {}; /* ZI because it omits a, b, c */
  struct B c = { argc, 2 }; /* ZI because it omits c[0], c[1], c[2] */

  /* ZI because it omits c[0], c[1] */
  struct B d = { .b = argc, .a = 2, .c[2].a = 3, .c[2].b = 4 };

  struct B e = { argc, 2, .c[2] = {}, .c[1] = { 9 }, .c[0] = {}, /* { dg-warning "padding might not be initialized to zero" } */
		 .c[0].a = 3, .c[0].b = 4, .c[1].a = 5, .c[1].b = 6,
		 .c[2].a = 7, .c[2].b = 8 };

  struct B f = { argc, 2, {}, /* { dg-warning "padding might not be initialized to zero" } */
		 .c[0].a = 3, .c[0].b = 4, .c[1].a = 5, .c[1].b = 6,
		 .c[2].a = 7, .c[2].b = 8 };

  struct B g = { argc, 2, .c[0].a = 3, .c[0].b = 4, .c[1].a = 5, .c[1].b = 6, /* { dg-warning "padding might not be initialized to zero" } */
		 .c[2].a = 7, .c[2].b = 8 };

  union U h = {}; /* ZI because it omits a, b */
  union U i = { argc }; /* { dg-warning "padding might not be initialized to zero" } */
  union U j = { .a = argc }; /* { dg-warning "padding might not be initialized to zero" } */
  union U k = { .b = argc }; /* Largest member is initialized */
  struct D l = {}; /* ZI because it omits a, b, c */
  struct D m = { argc, 2 }; /* ZI because it omits c */
  struct D n = { argc, 2, {}, .c.a.a = 3, .c.a.b = 4 }; /* { dg-warning "padding might not be initialized to zero" } */
  struct C o = { {} }; /* ZI because it omits a.a, a.b */
  struct C p = { {}, .a.a = argc, .a.b = 4 }; /* { dg-warning "padding might not be initialized to zero" } */
  struct E q = { argc, 2 }; /* No padding */
  union V r = { argc }; /* No padding */
  struct F s = { argc, {}, .b.a = 2 }; /* { dg-warning "padding might not be initialized to zero" } */
  struct F t = { argc, {}, .b = {.a = 2} }; /* { dg-warning "padding might not be initialized to zero" } */

  return 0;
}
