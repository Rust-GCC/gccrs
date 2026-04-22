// PR c++/124926
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

template <typename T, typename U>
constexpr bool is_same_v = false;

template <typename T>
constexpr bool is_same_v<T, T> = true;

// Class template
template <typename>
struct C1 {
  template <typename> struct Inner {};
};

template <typename T>
constexpr auto vt1 = ^^C1<T>::template Inner;
constexpr auto r1 = vt1<int>;
typename [:r1:]<int> a;
static_assert(r1 == ^^C1<int>::template Inner);
static_assert(is_same_v<decltype(a), C1<int>::Inner<int>>);

// Class template with typename
template <typename>
struct C6 {
  template <typename> struct Inner {};
};

template <typename T>
constexpr auto vt6 = ^^typename C6<T>::template Inner<T>;
constexpr auto r6 = vt6<int>;
typename [:r6:] d;
static_assert(r6 == ^^C6<int>::template Inner<int>);
static_assert(is_same_v<decltype(d), C6<int>::Inner<int>>);

// Variable template
template <typename>
struct C2 {
  template <typename> static constexpr int Inner = 42;
};

template <typename T>
constexpr auto vt2 = ^^C2<T>::template Inner;
constexpr auto r2 = vt2<int>;
constexpr int i = template [:r2:]<int>;
static_assert(i == 42);
static_assert(vt2<int> == ^^C2<int>::template Inner);

// Function template
template <typename>
struct C3 {
  template <typename T> static constexpr int Inner (T t) { return t; };
};
template <typename T>
constexpr auto vt3 = ^^C3<T>::template Inner;
constexpr auto r3 = vt3<int>;
static_assert(template [:r3:](42) == 42);
static_assert(vt3<int> == ^^C3<int>::template Inner);

// Alias template
template <typename>
struct C4 {
  template <typename T> using Inner = C4<T>;
};
template <typename T>
constexpr auto vt4 = ^^C4<T>::template Inner;
constexpr auto r4 = vt4<int>;
typename [:r4:]<int> b;
static_assert(is_same_v<decltype(b), C4<int>>);
static_assert(vt4<int> == ^^C4<int>::template Inner);

// Alias template with typename
template<typename T> struct X { };

template <typename>
struct C5 {
  template <typename T> using Inner = X<T>;
};
template <typename T>
constexpr auto vt5 = ^^typename C5<T>::template Inner<T>;
constexpr auto r5 = vt5<int>;
typename [:r5:] c;
static_assert(is_same_v<decltype(c), X<int>>);
static_assert(vt5<int> == ^^C5<int>::template Inner<int>);
