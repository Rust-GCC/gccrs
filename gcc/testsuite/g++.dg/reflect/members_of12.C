// PR c++/124645
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

namespace reg
{
template <auto namespace_info>
struct derived_type_registry
{
  static consteval auto
      reflected_types(std::meta::info scope = namespace_info)
  {
    std::vector<std::meta::info> infos;

    for(auto member : members_of(scope, std::meta::access_context::unchecked()))
    {
      if (member == std::meta::info{}) continue;
    }

    return infos;
  }
};
}

namespace ns1 {
    struct test {};
}

namespace ns2 {
}

static_assert(reg::derived_type_registry<^^ns1>::reflected_types().empty());
static_assert(reg::derived_type_registry<^^ns1>::reflected_types().size() == 0);
static_assert(reg::derived_type_registry<^^ns2>::reflected_types().empty());
static_assert(reg::derived_type_registry<^^ns2>::reflected_types().size() == 0);
