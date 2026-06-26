// PR c++/125900
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>
#include <print>

struct S {
    float f;
};

int main() {
    S s{.f = 1.5f};

    constexpr auto member{^^S::f};
    std::println("{}", s.[:member:]);
    std::println("{}", static_cast<int>(s.[:member:]));

    constexpr auto access_context{std::meta::access_context::current()};
    template for (constexpr auto member :
                  std::define_static_array(nonstatic_data_members_of(^^S, access_context))) {
        std::println("{}", s.[:member:]);
        std::println("{}", static_cast<int>(s.[:member:]));
    }
}
