// PR c++/124472
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

using foo = int;
namespace X {
    using ::foo;
    template<class> using bar = int;
}

X::bar<X::foo> y;
constexpr auto x = ^^X::bar<X::foo>;
