// PR c++/125378
// A version of operator-8a.C where the operator<=> returns long instead of int.

// { dg-do compile { target c++20 } }

struct A {
  bool operator==(int);
  long operator<=>(int);
};

template<class T>
void f() {
  A a;
  (void)(a != 0);
  (void)(0 != a);
  (void)(a < 0);
  (void)(0 < a);
  (void)(a <= 0);
  (void)(0 <= a);
  (void)(a > 0);
  (void)(0 > a);
  (void)(a >= 0);
  (void)(0 >= a);
}

// These later-declared namespace-scope overloads shouldn't be considered
// when instantiating f<int>.
bool operator!=(A, int) = delete;
bool operator<(A, int) = delete;
bool operator<=(A, int) = delete;
bool operator>(A, int) = delete;
bool operator>=(A, int) = delete;

bool operator!=(int, A) = delete;
bool operator<(int, A) = delete;
bool operator<=(int, A) = delete;
bool operator>(int, A) = delete;
bool operator>=(int, A) = delete;

template void f<int>();
