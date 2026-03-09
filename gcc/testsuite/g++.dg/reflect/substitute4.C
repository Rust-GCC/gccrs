// PR c++/124150
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>
using namespace std;

template <typename T>
consteval meta::info find_non_static_template_function() {
    for (auto r : meta::members_of(^^T, meta::access_context::current())) {
        if (meta::identifier_of(r) == "non_static_function") {
            return r;
        }
    }
    return ^^void;
}
template <typename T>
consteval meta::info find_static_template_member_func() {
    for (auto r : meta::members_of(^^T, meta::access_context::current())) {
        if (meta::identifier_of(r) == "static_function") {
            return r;
        }
    }
    return ^^void;
}
template <typename T>
consteval meta::info find_non_template_member_func() {
    for (auto r : meta::members_of(^^T, meta::access_context::current())) {
        if (meta::identifier_of(r) == "non_template_member_func") {
            return r;
        }
    }
    return ^^void;
}

template <typename T, typename Ts, meta::info static_func, meta::info non_static_func,
          meta::info non_template_func>
void call_member_template_functions(Ts* ptr) {
    [:substitute(static_func, {^^T}):]();
    ptr->[:substitute(non_static_func, {^^T}):]();
    ptr->[:non_template_func:]();
}

struct A {
    template <typename T>
    static void static_function() { }
    template <typename T>
    void non_static_function() { }
    void non_template_member_func() { }
};

template <typename T, meta::info r>
void call_non_member_template_function() {
    [:meta::substitute(r, {^^T}):]();
}

template <typename T>
void non_member_template_function() { }

int
main ()
{
    A a;
    call_member_template_functions<int, A, find_static_template_member_func<A>(),
                                   find_non_static_template_function<A>(),
                                   find_non_template_member_func<A>()>(&a);
    call_non_member_template_function<int, ^^non_member_template_function>();
}
