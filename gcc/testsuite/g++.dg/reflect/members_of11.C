// PR c++/124646
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

struct A { int a; };
struct B { int b; };

consteval auto nsdms(std::meta::info i) {
  return define_static_array(nonstatic_data_members_of(i, std::meta::access_context::unchecked()));
}

consteval void print_nsdms(std::meta::info i) {
    auto msg = std::string("members of ") + display_string_of(i) + " are:";
    for (auto mem : nsdms(i)) {
        msg += " ";
        msg += display_string_of(mem);
    }
    __builtin_constexpr_diag(0, "", msg); // { dg-message "members of B are: B::b" }
}

consteval {
    print_nsdms(^^A);
    print_nsdms(^^B);
}
