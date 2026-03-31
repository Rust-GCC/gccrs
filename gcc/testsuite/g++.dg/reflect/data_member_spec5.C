// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }
// Test std::meta::data_member_spec.

#include <meta>

using namespace std::meta;

consteval bool
valid_data_member_spec (info type, data_member_options opts)
{
  try { data_member_spec (type, opts); }
  catch (std::meta::exception &) { return false; }
  return true;
}

struct S { int a, b; };

consteval bool
foo ()
{
  constexpr int two = 2;
  [[=1]] int three = 3;
  constexpr int fourtytwo = 42;
  constexpr double fourtytwoandhalf = 42.5;
  data_member_options a = { .name = "_",
			    .annotations = { ^^two, reflect_constant (42),
					     annotations_of (^^three)[0],
					     reflect_constant (42.5),
					     reflect_constant (S { 1, 2 }) } };
  auto dmsa = data_member_spec (^^int, a);
  if (!is_data_member_spec (dmsa))
    throw 1;
  if (dmsa
      != data_member_spec (^^int,
			   { .name = "_",
			     .annotations = { reflect_constant (2),
					      ^^fourtytwo,
					      reflect_constant (1),
					      ^^fourtytwoandhalf,
					      reflect_constant (S { 1, 2 }) } }))
    throw 2;
  if (dmsa == data_member_spec (^^int, { .name = "_" }))
    throw 3;
  if (dmsa
      == data_member_spec (^^int,
			   { .name = "_",
			     .annotations = { reflect_constant (2L),
					      ^^fourtytwo,
					      reflect_constant (1),
					      ^^fourtytwoandhalf,
					      reflect_constant (S { 1, 2 }) } }))
    throw 4;
  if (dmsa
      == data_member_spec (^^int,
			   { .name = "_",
			     .annotations = { reflect_constant (2),
					      ^^two,
					      reflect_constant (1),
					      ^^fourtytwoandhalf,
					      reflect_constant (S { 1, 2 }) } }))
    throw 4;
  if (dmsa
      == data_member_spec (^^int,
			   { .name = "_",
			     .annotations = { reflect_constant (2),
					      ^^fourtytwo,
					      reflect_constant (3),
					      ^^fourtytwoandhalf,
					      reflect_constant (S { 1, 2 }) } }))
    throw 5;
  if (dmsa
      == data_member_spec (^^int,
			   { .name = "_",
			     .annotations = { reflect_constant (2),
					      ^^fourtytwo,
					      reflect_constant (1),
					      reflect_constant (42.25),
					      reflect_constant (S { 1, 2 }) } }))
    throw 6;
  if (dmsa
      == data_member_spec (^^int,
			   { .name = "_",
			     .annotations = { reflect_constant (2),
					      ^^fourtytwo,
					      reflect_constant (1),
					      reflect_constant (42.25),
					      reflect_constant (S { 1, 2 }) } }))
    throw 7;
  if (dmsa
      == data_member_spec (^^int,
			   { .name = "_",
			     .annotations = { reflect_constant (2),
					      ^^fourtytwo,
					      reflect_constant (1),
					      reflect_constant (42.5),
					      reflect_constant (S { 2, 2 }) } }))
    throw 8;
  return true;
}

static_assert (foo ());

constexpr int arr[1] = { 1 };
constexpr int i = 42;
constexpr S s = { 42, 43 };

static_assert (!valid_data_member_spec (^^int, { .name = "a", .annotations = { ^^:: } }));
static_assert (!valid_data_member_spec (^^int, { .name = "a", .annotations = { ^^foo } }));
static_assert (!valid_data_member_spec (^^int, { .name = "a", .annotations = { ^^arr } }));
static_assert (!valid_data_member_spec (^^int, { .name = "a", .annotations = { ^^arr } }));
static_assert (valid_data_member_spec (^^int, { .name = "a", .annotations = { ^^i } }));
static_assert (valid_data_member_spec (^^int, { .name = "a", .annotations = { ^^s } }));
static_assert (!valid_data_member_spec (^^int, { .bit_width = 0, .annotations = { ^^i } }));
