// PR c++/105667
// { dg-do compile { target c++20 } }

template<auto f>
struct h {typedef int type;};

template<class Types>
struct t{};

template<int Ts>
struct crash {
    using Types = typename h<[]() {}>::type;
    template<int tt>
    static void f()
    {
        t<Types> b;
    };
};

int main() {
    crash<0>::f<1>();
}
