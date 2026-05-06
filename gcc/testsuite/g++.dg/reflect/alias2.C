// PR c++/124544
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

template<auto V, class T>
struct A;

template<auto V>
struct A<V, char> {
  static void f();
};

template<class T> using alias_t = T;

int main() {
  A<^^int, char>::f();
  A<^^alias_t<int>, char>::f();
}
