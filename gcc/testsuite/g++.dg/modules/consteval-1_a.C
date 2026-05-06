// PR c++/124973
// { dg-do compile { target c++20 } }
// { dg-additional-options "-fmodules" }

export module right;		// { dg-module-cmi right }

struct constexpr_move {
    constexpr_move() = default;
    constexpr_move(const constexpr_move&) = default;
    constexpr constexpr_move(constexpr_move&&) {}
};

struct element {
    constexpr_move a;
    int b;
};

template <unsigned N>
struct element_array {
    element data[ N ];
};

template <unsigned N>
struct holder {
    consteval holder(element_array<N> values) :
        values_{static_cast<element_array<N>&&>(values)} {}
    element_array<N> values_;
};

template <unsigned N>
constexpr auto holder_of = holder<N>{element_array<N>{}};

export class environment {
    decltype(holder_of<1>) s1_{holder_of<1>};
    decltype(holder_of<2>) s2_{holder_of<2>};
};
