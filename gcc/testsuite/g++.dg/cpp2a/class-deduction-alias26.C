// PR c++/122621
// { dg-do compile { target c++20 } }

template <class, class>
concept constructible_from = true;

template <class... ElemTs>
struct Tuple {
    template <class... Ts> requires (... && constructible_from<ElemTs, Ts>)
    Tuple(Ts &&...) {}
};

template <class... Us>
using A = Tuple<Tuple<Us...>>;

using type = decltype(A{0});
using type = Tuple<Tuple<>>;
