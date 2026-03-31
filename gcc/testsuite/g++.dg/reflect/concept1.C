// PR c++/124457
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

template <class T, auto t = ^^T>
concept True = true;

template <class T>
concept AlsoTrue = requires (T t) {
    { t } -> True;
};
void f1(True auto x);
template <True T> void f2(T );
auto f3(int) -> True auto;
