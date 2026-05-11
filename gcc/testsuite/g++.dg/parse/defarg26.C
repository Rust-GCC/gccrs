// PR c++/50479
// { dg-do compile { target c++11 } }

template<class>
struct A;

template<>
struct A<long> {
  static const int value = 1;
};

long foo;

void bar(char foo = A<decltype(foo)>::value) {} // { dg-error "incomplete type" }

int main() {
  bar();
}
