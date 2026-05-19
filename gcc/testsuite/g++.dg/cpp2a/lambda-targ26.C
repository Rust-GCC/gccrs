// PR c++/125212
// { dg-do compile { target c++20 } }

template <auto dummyVal>
struct MyStruct
{
    template<class Callable>
    static constexpr auto Invoke(Callable callable)
    {
        return callable.template operator() <decltype(val)> ();
    }

    static constexpr int val = 0;

    template <auto callable>
    using Alias = MyStruct<dummyVal>;
};

template <typename T>
constexpr auto IceFunction()
{
    using Alias1 = T::template Alias<[]<typename>() {}>;
    constexpr auto lambda = []<typename>() {};
    using Alias2 = T::template Alias<lambda>;
    using Alias = Alias1;
    return Alias::Invoke([]<typename>{ return MyStruct<Alias::val>{}; });
}

int main()
{
    IceFunction<MyStruct<1>>();
}
