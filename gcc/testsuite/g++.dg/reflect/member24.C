// PR c++/124989
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

using info = decltype(^^::);

template <info R>
constexpr auto pointer_of()
{
  return &[: R :];
}

struct X
{
    int a;
protected:
    int b;
private:
    int c;

public:
    static constexpr auto pa = pointer_of<^^X::a>();
    static constexpr auto pb = pointer_of<^^X::b>();
    static constexpr auto pc = pointer_of<^^X::c>();
};
