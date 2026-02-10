/* Test that no C23 warnings are produced about initializers of objects with
 * static storage duration that might not have zeroed padding bits if they
 * instead had automatic storage duration.
 */
/* { dg-do run } */
/* { dg-options "-std=c23 -Wzero-init-padding-bits=all" } */

struct A { unsigned char a; long long b; };
struct B { unsigned char a; long long b; struct A c[3]; };
struct C { struct A a; };
struct D { unsigned char a; long long b; struct C c; };
struct E { long long a; long long b; };
union U { unsigned char a; long long b; };
union V { long long a; long long b; };
struct F { long long a; union U b; };

int
main ()
{
  static struct A a = {};
  static struct B b = {};
  static struct B c = { 1, 2 };
  static struct B d = { .b = 1, .a = 2, .c[2].a = 3, .c[2].b = 4 };

  static struct B e = { 1, 2, .c[2] = {}, .c[1] = { 9 }, .c[0] = {},
			.c[0].a = 3, .c[0].b = 4, .c[1].a = 5,
			.c[1].b = 6, .c[2].a = 7, .c[2].b = 8 };

  static struct B f = { 1, 2, {},
			.c[0].a = 3, .c[0].b = 4, .c[1].a = 5,
			.c[1].b = 6, .c[2].a = 7, .c[2].b = 8 };

  static struct B g = { 1, 2, .c[0].a = 3, .c[0].b = 4, .c[1].a = 5,
			.c[1].b = 6, .c[2].a = 7, .c[2].b = 8 };

  static union U h = {};
  static union U i = { 1 };
  static union U j = { .a = 1 };
  static union U k = { .b = 1 };
  static struct D l = {};
  static struct D m = { 1, 2 };
  static struct D n = { 1, 2, {}, .c.a.a = 3, .c.a.b = 4 };
  static struct C o = { {} };
  static struct C p = { {}, .a.a = 3, .a.b = 4 };
  static struct E q = { 1, 2 };
  static union V r = { 1 };
  static struct F s = { 1, {}, .b.a = 2 };
  static struct F t = { 1, {}, .b = {.a = 2} };

  return 0;
}
