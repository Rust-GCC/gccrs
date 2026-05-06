// PR c++/124835
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

template <typename T, typename U>
constexpr bool is_same_v = false;

template <typename T>
constexpr bool is_same_v<T, T> = true;

template<typename T>
constexpr T foo (T t) { return t; }

template<typename T>
struct S { };

static_assert([:^^foo<int>:](42) == 42);
static_assert(is_same_v<decltype([:^^foo<int>:]), int(int)>);
static_assert(is_same_v<decltype([:^^foo<int>:](42)), int>);
static_assert(is_same_v<decltype(template [:^^foo:](0)), int>);
static_assert(is_same_v<decltype(template [:^^foo:]<int>(0)), int>);
static_assert(is_same_v<decltype(([:^^foo<int>:])), int(&)(int)>);
static_assert(is_same_v<decltype(([:^^foo<int>:](42))), int>);
static_assert(is_same_v<decltype((template [:^^foo:](0))), int>);
static_assert(is_same_v<decltype((template [:^^foo:]<int>(0))), int>);
