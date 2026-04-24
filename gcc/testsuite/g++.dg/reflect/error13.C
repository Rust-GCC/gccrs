// PR c++/124756
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

template<typename T>
consteval void
f0 ()
{
  void fun(T);
  auto _ = ^^fun;  // { dg-error "cannot take the reflection of a block-scope extern .fun." }
  extern int arr[3];
  auto _ = ^^arr;  // { dg-error "cannot take the reflection of a block-scope extern .arr." }
  extern int e;
  auto _ = ^^e;  // { dg-error "cannot take the reflection of a block-scope extern .e." }
}

consteval {
  f0<int>();
}

consteval void
f1 ()
{
  void fun(int);
  auto _ = ^^fun;  // { dg-error "cannot take the reflection of a block-scope extern .fun." }
  extern int arr[3];
  auto _ = ^^arr;  // { dg-error "cannot take the reflection of a block-scope extern .arr." }
  extern int e;
  auto _ = ^^e;  // { dg-error "cannot take the reflection of a block-scope extern .e." }
}
