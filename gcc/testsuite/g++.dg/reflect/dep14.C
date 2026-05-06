// PR c++/123783
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>
#include <ranges>

namespace __impl {
  template<auto... vals>
  struct replicator_type {
    template<typename F>
      constexpr void operator>>(F body) const {
        (body.template operator()<vals>(), ...);
      }
  };

  template<auto... vals>
  replicator_type<vals...> replicator = {};
}

template<typename R>
consteval auto expand(R range) {
  std::vector<std::meta::info> args;
  for (auto r : range) {
    args.push_back(reflect_constant(r));
  }
  return substitute(^^__impl::replicator, args);
}

int func(int counter, float factor) {
    std::array<void *, parameters_of(^^func).size()> args;

    std::size_t i = 0;
    [:expand(parameters_of(^^func) | std::views::transform(std::meta::variable_of)):] >> [&i, &counter, &factor, &args]<auto e>
    {
        args[i++] = &[:e:]; // { dg-error "use of local variable" }
    };
    return 0;
}

int func2(int counter, float factor) {
    std::array<void *, parameters_of(^^func2).size()> args;

    std::size_t i = 0;
    [:expand(parameters_of(^^func2) | std::views::transform(std::meta::variable_of)):] >> [&]<auto e>
    {
        args[i++] = &[:e:]; // { dg-error "use of local variable|trying to capture \[^\n\r]* in instantiation of generic lambda" }
    };
    return 0;
}

int main()
{
    func(11, 22);
    func(33, 44);
    func2(11, 22);
    func2(33, 44);
}
