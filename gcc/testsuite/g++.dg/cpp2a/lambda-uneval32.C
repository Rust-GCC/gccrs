// PR c++/106957
// { dg-do compile { target c++20 } }

template<bool B>
struct bool_constant {
  static constexpr bool value = B;
  constexpr operator bool() const { return value; }
};

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

constexpr auto f = [](int){}; // { dg-error "invalid conversion" }

template<typename T>
auto canCallFOn(T) -> decltype([]{ f(T{}); }(), true_type{}); // { dg-error "no match|invalid conversion" }

constexpr false_type canCallFOn(...) {
    return {};
}

int main() {
    static_assert(!canCallFOn(1)); // { dg-error "" }
    static_assert(!canCallFOn("")); // { dg-error "" }
}
