// PR c++/124981
// { dg-additional-options "-fmodules" }
// { dg-module-cmi M }

export module M;

template<class T, void (T::*F)() const>
struct A {
  void f(const T& t) {
    (t.*F)();
  }
};

export
template<class T>
struct B {
  void g() const { }
  A<B, &B::g> m;
};

B<void> b;
