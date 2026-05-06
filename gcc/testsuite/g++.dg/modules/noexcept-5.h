// PR c++/124785
template <typename T> struct Iterator {
  constexpr friend auto operator<=>(const Iterator&, const Iterator&) = default;
};
