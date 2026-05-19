// PR c++/105667
// { dg-do compile { target c++20 } }

template<typename>
struct get { };

template<int>
struct thing {
    using T = decltype([](auto) { });

    static constexpr auto value = [](auto) {
        return get<T>();
    }(0);
};

thing<0> X;
