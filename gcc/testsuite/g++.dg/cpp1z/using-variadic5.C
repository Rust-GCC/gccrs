// PR c++/125333
// { dg-do compile { target c++17 } }

template<typename... T>
struct X {};

template <typename U>
struct Y {
    operator U();
};

template <typename... V>
struct Z : Y<V>... {
    using Y<V>::operator V...;

    X<V...> x;
};

Z<int, double> z;
