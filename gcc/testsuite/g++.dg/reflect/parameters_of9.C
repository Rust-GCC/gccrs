// PR c++/125498
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection -g" }

#include <meta>

struct tag {};

struct S {
  S(int) {}
};

struct B { };
struct A : virtual B { A(int); };

template <auto Param> struct GetAnnotations {
  static constexpr auto value = std::define_static_array(
      std::meta::annotations_of_with_type(Param, ^^tag));
};

consteval bool test() {
  template for (constexpr auto member :
		std::define_static_array(std::meta::members_of(
		    ^^S, std::meta::access_context::unchecked()))) {
    if constexpr (std::meta::is_constructor(member)) {
      constexpr auto parameters =
	  std::define_static_array(std::meta::parameters_of(member));
      constexpr auto annotations = GetAnnotations<parameters[0]>::value;
      return annotations.empty();
    }
  }
}

consteval bool test2() {
  template for (constexpr auto member :
		std::define_static_array(std::meta::members_of(
		    ^^A, std::meta::access_context::unchecked()))) {
    if constexpr (std::meta::is_constructor(member)) {
      constexpr auto parameters =
	  std::define_static_array(std::meta::parameters_of(member));
      constexpr auto annotations = GetAnnotations<parameters[0]>::value;
      return annotations.empty();
    }
  }
}
