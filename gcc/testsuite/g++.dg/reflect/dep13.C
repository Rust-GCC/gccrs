// PR c++/124617
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

template <class F>
struct S {
    template <int I>
    static constexpr int visit(F&&) { return 42; }

    using Ptr = decltype(&visit<0>);
    static constexpr std::meta::info fptrs = ^^visit<0>;
};

template <class F>
constexpr int foo(F&& f) {
    using Impl = S<F>;
    auto which = Impl::fptrs;
    auto func = std::meta::extract<typename Impl::Ptr>(which);
    return func((F&&)f);
}

struct Plus { };
static_assert(foo(Plus{}) == 42);
