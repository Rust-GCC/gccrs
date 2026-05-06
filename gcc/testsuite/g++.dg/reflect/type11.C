// PR c++/124855
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

using mi = decltype(^^::);

template<class T>
auto f(T) -> [: ^^T == ^^int ? ^^char : mi{} :];

static_assert(sizeof(f(0)) == 1);
