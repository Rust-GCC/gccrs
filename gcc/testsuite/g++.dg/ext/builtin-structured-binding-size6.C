// PR c++/124862
// { dg-do compile { target c++20 } }

struct S
{
  int a = 0, b = 0, c = 0;
};

struct T : private S
{};

template <typename T>
concept is_destructurable = requires { { __builtin_structured_binding_size (T) }; };

static_assert (is_destructurable <S>);
static_assert (!is_destructurable <T>);
