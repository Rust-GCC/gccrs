// PR c++/125939
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

template<typename T>
constexpr bool cct = std::meta::is_copy_constructible_type(^^T);

int main() {
    using T = int&;
    static_assert(std::meta::is_copy_constructible_type(^^T));
    static_assert(cct<int&>);
}

static_assert (is_copy_constructible_type (^^int));
static_assert (std::is_copy_constructible_v<int>);
static_assert (is_copy_constructible_type (^^const int));
static_assert (std::is_copy_constructible_v<const int>);
static_assert (is_copy_constructible_type (^^int &));
static_assert (std::is_copy_constructible_v<int &>);
static_assert (is_copy_constructible_type (^^const int &));
static_assert (std::is_copy_constructible_v<const int &>);
static_assert (!is_copy_constructible_type (^^int &&));
static_assert (!std::is_copy_constructible_v<int &&>);
static_assert (!is_copy_constructible_type (^^const int &&));
static_assert (!std::is_copy_constructible_v<const int &&>);
static_assert (!is_copy_constructible_type (^^void));
static_assert (!std::is_copy_constructible_v<void>);
static_assert (!is_copy_constructible_type (^^int() const &));
static_assert (!std::is_copy_constructible_v<int() const &>);

static_assert (is_trivially_copy_constructible_type (^^int));
static_assert (std::is_trivially_copy_constructible_v<int>);
static_assert (is_trivially_copy_constructible_type (^^const int));
static_assert (std::is_trivially_copy_constructible_v<const int>);
static_assert (is_trivially_copy_constructible_type (^^int &));
static_assert (std::is_trivially_copy_constructible_v<int &>);
static_assert (is_trivially_copy_constructible_type (^^const int &));
static_assert (std::is_trivially_copy_constructible_v<const int &>);
static_assert (!is_trivially_copy_constructible_type (^^int &&));
static_assert (!std::is_trivially_copy_constructible_v<int &&>);
static_assert (!is_trivially_copy_constructible_type (^^const int &&));
static_assert (!std::is_trivially_copy_constructible_v<const int &&>);
static_assert (!is_trivially_copy_constructible_type (^^void));
static_assert (!std::is_trivially_copy_constructible_v<void>);
static_assert (!is_trivially_copy_constructible_type (^^int() const &));
static_assert (!std::is_trivially_copy_constructible_v<int() const &>);

static_assert (is_nothrow_copy_constructible_type (^^int));
static_assert (std::is_nothrow_copy_constructible_v<int>);
static_assert (is_nothrow_copy_constructible_type (^^const int));
static_assert (std::is_nothrow_copy_constructible_v<const int>);
static_assert (is_nothrow_copy_constructible_type (^^int &));
static_assert (std::is_nothrow_copy_constructible_v<int &>);
static_assert (is_nothrow_copy_constructible_type (^^const int &));
static_assert (std::is_nothrow_copy_constructible_v<const int &>);
static_assert (!is_nothrow_copy_constructible_type (^^int &&));
static_assert (!std::is_nothrow_copy_constructible_v<int &&>);
static_assert (!is_nothrow_copy_constructible_type (^^const int &&));
static_assert (!std::is_nothrow_copy_constructible_v<const int &&>);
static_assert (!is_nothrow_copy_constructible_type (^^void));
static_assert (!std::is_nothrow_copy_constructible_v<void>);
static_assert (!is_nothrow_copy_constructible_type (^^int() const &));
static_assert (!std::is_nothrow_copy_constructible_v<int() const &>);

static_assert (!is_assignable_type (^^int, ^^int));
static_assert (!std::is_assignable_v<int, int>);
static_assert (!is_assignable_type (^^const int, ^^int));
static_assert (!std::is_assignable_v<const int, int>);
static_assert (is_assignable_type (^^int &, ^^int));
static_assert (std::is_assignable_v<int &, int>);
static_assert (!is_assignable_type (^^const int &, ^^int));
static_assert (!std::is_assignable_v<const int &, int>);
static_assert (!is_assignable_type (^^int &&, ^^int));
static_assert (!std::is_assignable_v<int &&, int>);
static_assert (!is_assignable_type (^^const int &&, ^^int));
static_assert (!std::is_assignable_v<const int &&, int>);
static_assert (!is_assignable_type (^^void, ^^int));
static_assert (!std::is_assignable_v<void, int>);
static_assert (!is_assignable_type (^^int() const &, ^^int));
static_assert (!std::is_assignable_v<int() const &, int>);

static_assert (is_copy_assignable_type (^^int));
static_assert (std::is_copy_assignable_v<int>);
static_assert (!is_copy_assignable_type (^^const int));
static_assert (!std::is_copy_assignable_v<const int>);
static_assert (is_copy_assignable_type (^^int &));
static_assert (std::is_copy_assignable_v<int &>);
static_assert (!is_copy_assignable_type (^^const int &));
static_assert (!std::is_copy_assignable_v<const int &>);
static_assert (is_copy_assignable_type (^^int &&));
static_assert (std::is_copy_assignable_v<int &&>);
static_assert (!is_copy_assignable_type (^^const int &&));
static_assert (!std::is_copy_assignable_v<const int &&>);
static_assert (!is_copy_assignable_type (^^void));
static_assert (!std::is_copy_assignable_v<void>);
static_assert (!is_copy_assignable_type (^^int() const &));
static_assert (!std::is_copy_assignable_v<int() const &>);

static_assert (is_trivially_copy_assignable_type (^^int));
static_assert (std::is_trivially_copy_assignable_v<int>);
static_assert (!is_trivially_copy_assignable_type (^^const int));
static_assert (!std::is_trivially_copy_assignable_v<const int>);
static_assert (is_trivially_copy_assignable_type (^^int &));
static_assert (std::is_trivially_copy_assignable_v<int &>);
static_assert (!is_trivially_copy_assignable_type (^^const int &));
static_assert (!std::is_trivially_copy_assignable_v<const int &>);
static_assert (is_trivially_copy_assignable_type (^^int &&));
static_assert (std::is_trivially_copy_assignable_v<int &&>);
static_assert (!is_trivially_copy_assignable_type (^^const int &&));
static_assert (!std::is_trivially_copy_assignable_v<const int &&>);
static_assert (!is_trivially_copy_assignable_type (^^void));
static_assert (!std::is_trivially_copy_assignable_v<void>);
static_assert (!is_trivially_copy_assignable_type (^^int() const &));
static_assert (!std::is_trivially_copy_assignable_v<int() const &>);

static_assert (is_nothrow_copy_assignable_type (^^int));
static_assert (std::is_nothrow_copy_assignable_v<int>);
static_assert (!is_nothrow_copy_assignable_type (^^const int));
static_assert (!std::is_nothrow_copy_assignable_v<const int>);
static_assert (is_nothrow_copy_assignable_type (^^int &));
static_assert (std::is_nothrow_copy_assignable_v<int &>);
static_assert (!is_nothrow_copy_assignable_type (^^const int &));
static_assert (!std::is_nothrow_copy_assignable_v<const int &>);
static_assert (is_nothrow_copy_assignable_type (^^int &&));
static_assert (std::is_nothrow_copy_assignable_v<int &&>);
static_assert (!is_nothrow_copy_assignable_type (^^const int &&));
static_assert (!std::is_nothrow_copy_assignable_v<const int &&>);
static_assert (!is_nothrow_copy_assignable_type (^^void));
static_assert (!std::is_nothrow_copy_assignable_v<void>);
static_assert (!is_nothrow_copy_assignable_type (^^int() const &));
static_assert (!std::is_nothrow_copy_assignable_v<int() const &>);
