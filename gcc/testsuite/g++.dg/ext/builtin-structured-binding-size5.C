// PR c++/124862
// { dg-do compile { target c++20 } }

struct S
{
  int a = 0, b = 0, c = 0;
};

struct T : private S
{};

static_assert (__builtin_structured_binding_size (S) == 3);
auto a = __builtin_structured_binding_size (T);		// { dg-error "'S' is an inaccessible base of 'T'" }
