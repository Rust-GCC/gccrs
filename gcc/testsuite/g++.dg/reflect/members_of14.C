// PR c++/124628
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

template<class T> void f();

template<class T>
struct A {
  void g() noexcept(noexcept(T{}));
  auto h() { return T{}; }
  static inline auto m = T{};
  // A& operator=(const A&) noexcept;
  // A& operator=(A&&) noexcept;
};

int main() {
  constexpr auto ac = std::meta::access_context::current();
  template for (constexpr auto mem : define_static_array(members_of(^^A<int>, ac)))
    if constexpr (!is_constructor(mem) && !is_destructor(mem))
      f<typename [:type_of(mem):]>();
}

// { dg-final { scan-assembler _Z1fIDoFvvEEvv } } void f<void () noexcept>()
// { dg-final { scan-assembler _Z1fIFivEEvv } } void f<int ()>()
// { dg-final { scan-assembler _Z1fIiEvv } } void f<int>()
// { dg-final { scan-assembler _Z1fIDoFR1AIiERKS1_EEvv } } void f<A<int>& (A<int> const&) noexcept>()
// { dg-final { scan-assembler _Z1fIDoFR1AIiEOS1_EEvv } } void f<A<int>& (A<int>&&) noexcept>()
