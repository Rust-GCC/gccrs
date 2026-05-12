// PR c++/125280
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

template <class T, int N>
struct Array {
  T mElements[N]; // { dg-error "incomplete type" }
};

struct Element {
  static const Array<Element, 10> mData{}; // { dg-error "initialization of static data member" }
};
const Array<Element, 10> Element::mData;
