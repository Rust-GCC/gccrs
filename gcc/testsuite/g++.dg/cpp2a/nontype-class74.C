// PR c++/124978
// { dg-do compile { target c++20 } }

template<auto X, class = decltype(X)>
struct cw { static constexpr const auto& value = X; };

template<class L, class R>
auto f(L, R) -> cw<(L::value ->* R::value)>;

struct Divide { int value; };
auto co = cw<&cw<Divide{42}>::value>{};
auto cm = cw<&Divide::value>{};

using type = decltype(f(co, cm));
using type = cw<42, const int>; // FIXME PR115314, unexpected 'const'
