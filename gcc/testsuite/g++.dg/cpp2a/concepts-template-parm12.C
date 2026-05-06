// { dg-do compile { target c++20 } }
// Verify partial ordering with respect to associated constraints
// works in the presence of constrained NTTPs.

template<class T> concept C = true;

template<class T> concept D = C<T> && true;

template<class T> concept E = true;

template<C auto V> void f() = delete;
template<D auto V> void f(); // more constrained

template<C auto V> void g();
template<C auto V> void g(); // redeclaration

template<C auto V> void h();
template<E auto V> void h(); // ambiguous

template<C auto V> struct A;
template<D auto V> struct A<V> { }; // more constrained

template<D auto V> struct B;
template<C auto V> struct B<V> { }; // { dg-error "not more constrained" }

int main() {
  f<0>();
  g<0>();
  h<0>(); // { dg-error "ambiguous" }
  A<0> a;
}
