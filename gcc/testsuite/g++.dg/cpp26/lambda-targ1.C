// PR c++/121287
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

template <typename _Iter>
concept random_access_iterator = requires(_Iter __n) { __n; };
template <typename> struct default_accessor {};
template <random_access_iterator I> struct iterator_accessor {
  using element_type = [:[] { return ^^I; }():];
  operator default_accessor<const element_type>();
};
default_accessor<const int> _ = iterator_accessor<int>();
