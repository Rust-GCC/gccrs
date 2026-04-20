// PR c++/124950
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection -Wmissing-field-initializers" }
// Test that we don't emit bogus -Wmissing-field-initializers warnings.

#include <meta>
using namespace std::meta;
constexpr auto dms = data_member_spec (^^int, { .name = "dms" });
constexpr auto dms2 = data_member_spec (^^int, { .bit_width = 8 }); // { dg-warning "missing initializer" }
