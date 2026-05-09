// PR c++/125007
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

[[=1]] void foo (int x, int y);
[[=2]] void foo (int x, int z);

struct A { int a; };
struct B { int b; };
struct C : public A, B {};
struct D : public A, B {};

constexpr auto ctx = std::meta::access_context::unchecked ();
static_assert (annotations_of (^^foo)[0] == annotations_of (^^foo)[1]);	// { dg-error "static assertion failed" }
// { dg-message "note: the comparison reduces to '\\\(\\\^\\\^\\\[\\\[=1\\\]\\\] == \\\^\\\^\\\[\\\[=2\\\]\\\]\\\)'" "" { target *-*-* } .-1 }
static_assert (parameters_of (^^foo)[0] == parameters_of (^^foo)[1]);	// { dg-error "static assertion failed" }
// { dg-message "note: the comparison reduces to '\\\(parameters_of\\\(\\\^\\\^foo\\\(int, int\\\)\\\)\\\[0\\\] \\\{aka x\\\} == parameters_of\\\(\\\^\\\^foo\\\(int, int\\\)\\\)\\\[1\\\]\\\)'" "" { target *-*-* } .-1 }
static_assert (bases_of (^^C, ctx)[0] == bases_of (^^C, ctx)[1]);	// { dg-error "static assertion failed" }
// { dg-message "note: the comparison reduces to '\\\(bases_of\\\(\\\^\\\^C,std::meta::access_context::unchecked\\\(\\\)\\\)\\\[0\\\] \\\{aka A\\\} == bases_of\\\(\\\^\\\^C,std::meta::access_context::unchecked\\\(\\\)\\\)\\\[1\\\] \\\{aka B\\\}\\\)'" "" { target *-*-* } .-1 }
static_assert (bases_of (^^C, ctx)[1] == bases_of (^^D, ctx)[1]);	// { dg-error "static assertion failed" }
// { dg-message "note: the comparison reduces to '\\\(bases_of\\\(\\\^\\\^C,std::meta::access_context::unchecked\\\(\\\)\\\)\\\[1\\\] \\\{aka B\\\} == bases_of\\\(\\\^\\\^D,std::meta::access_context::unchecked\\\(\\\)\\\)\\\[1\\\] \\\{aka B\\\}\\\)'" "" { target *-*-* } .-1 }
constexpr auto an1 = annotations_of (^^foo)[0];
constexpr auto an2 = annotations_of (^^foo)[1];
constexpr auto an3 = std::meta::reflect_constant (3);
static_assert (data_member_spec (^^int, { .name = "foo", .alignment = 64, .no_unique_address = true, .annotations = { an1, an2, an3 } }) == ^^::);	// { dg-error "static assertion failed" }
// { dg-message "note: the comparison reduces to '\\\(data_member_spec\\\(\\\^\\\^int,\\\{.name=\\\"foo\\\",.alignment=64,.no_unique_address=true,.annotations=\\\{std::meta::reflect_constant\\\(1\\\),std::meta::reflect_constant\\\(2\\\),std::meta::reflect_constant\\\(3\\\)\\\}\\\}\\\) == \\\^\\\^::\\\)'" "" { target *-*-* } .-1 }
static_assert (data_member_spec (^^int, { .bit_width = 5 }) == ^^::);	// { dg-error "static assertion failed" }
// { dg-message "note: the comparison reduces to '\\\(data_member_spec\\\(\\\^\\\^int,\\\{.bit_width=5\\\}\\\) == \\\^\\\^::\\\)'" "" { target *-*-* } .-1 }
static_assert (std::meta::reflect_constant (42) == std::meta::reflect_constant (A { 42 })); // { dg-error "static assertion failed" }
// { dg-message "note: the comparison reduces to '\\\(std::meta::reflect_constant\\\(42\\\) == std::meta::reflect_object\\\(A\\\{42\\\}\\\)\\\)'" "" { target *-*-* } .-1 }
int v[42];
static_assert (std::meta::reflect_object (v[4]) == std::meta::reflect_object (v[5])); // { dg-error "static assertion failed" }
// { dg-message "note: the comparison reduces to '\\\(std::meta::reflect_object\\\(v\\\[4\\\]\\\) == std::meta::reflect_object\\\(v\\\[5\\\]\\\)\\\)'" "" { target *-*-* } .-1 }

void
qux (int, int)
{
}

void qux (int x, int y);
void qux (int x, int y);

static_assert (parameters_of (^^qux)[0] == parameters_of (^^qux)[1]);	// { dg-error "static assertion failed" }
// { dg-message "note: the comparison reduces to '\\\(parameters_of\\\(\\\^\\\^qux\\\(int, int\\\)\\\)\\\[0\\\] \\\{aka x\\\} == parameters_of\\\(\\\^\\\^qux\\\(int, int\\\)\\\)\\\[1\\\] \\\{aka y\\\}\\\)'" "" { target *-*-* } .-1 }

static_assert (std::meta::info{} == ^^int);	// { dg-error "static assertion failed" }
// { dg-message {note: the comparison reduces to '\(std::meta::info{} == \^\^int\)} "" { target *-*-* } .-1 }
