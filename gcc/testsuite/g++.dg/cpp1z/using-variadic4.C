// PR c++/125284
// { dg-do compile { target c++17 } }

template<typename T>
struct foo {
    operator T();
    operator T*();
};

template<typename... T>
struct bar : foo<T>... {
    using foo<T>::operator T...;
    using foo<T>::operator T*...;
};

bar<int> z;
