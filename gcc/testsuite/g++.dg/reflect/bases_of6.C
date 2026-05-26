// PR c++/125423
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

struct Base
{
};
struct Derived : Base
{
};

template <typename Tp, Tp v> struct get_type
{
    inline constexpr static auto info = ^^void;
};

template <std::meta::info base> struct get_type<std::meta::info, base>
{
    inline constexpr static auto info = std::meta::type_of(base);
};

template <std::meta::info currField> struct ReflectionSubPanel
{
    static constexpr auto _ranges = []()
    {
        constexpr auto b = std::meta::bases_of(^^Derived, std::meta::access_context::unchecked())[0];
        return get_type<decltype(b), b>::info;
    }();
};

int main() { ReflectionSubPanel<^^Derived> panel; }
