// PR c++/125408
// { dg-do compile { target c++20 } }

struct Role {
  long id;
};

template <typename T>
struct StaticObj {
  inline static T obj;
};

template<int... I> struct A { };

template <auto Ptr, typename Table>
constexpr int find_member_index() {
  constexpr auto& t = StaticObj<Table>::obj;

  [&]<int... Idx>(A<Idx...>) {
    ([&]{
      using T1 = decltype(&(t.*Ptr));
      auto p = &(t.*Ptr);
      return Idx;
    }(), ...);
  }(A<0,1>{});
  return 42;
}

auto p = find_member_index<&Role::id, Role>();

int main() {}
