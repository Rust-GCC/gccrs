/* PR c++/124573 */
/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-require-effective-target c++11 } */

template <typename T>
struct A {
    enum E : T;
};

using X = A<int>;

template <typename T>
enum X::E :  /* { dg-error "cannot add an enumerator list" } */
  T { e1 };  /* { dg-error "different underlying type" } */

template <>
enum A<int>::E : int {};

