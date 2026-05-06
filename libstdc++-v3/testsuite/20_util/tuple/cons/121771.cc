// { dg-do compile { target c++17 } }

// Bug 121771 - std::tuple CTAD fails for lvalue reference to function type

#include <tuple>
#include <memory>

void func();

std::tuple t(func);
std::tuple<void (*)()>& r = t;

struct Explicit {
    Explicit();
    explicit Explicit(const Explicit&);
} ex;

std::tuple t2(func, 1);
std::tuple<void (*)(), int>& r2 = t2;

std::tuple t2x(ex, func);
std::tuple<Explicit, void (*)()>& r2x = t2x;

std::tuple t3(1, func, 3);
std::tuple<int, void (*)(), int>& r3 = t3;

std::tuple t3x(ex, 2, func);
std::tuple<Explicit, int, void (*)()>& r3x = t3x;

std::allocator<int> alloc;

std::tuple ta(std::allocator_arg, alloc, func);
std::tuple<void (*)()>& ra = ta;

std::tuple ta2(std::allocator_arg, alloc, func, 1);
std::tuple<void (*)(), int>& ra2 = ta2;

std::tuple ta2x(std::allocator_arg, alloc, ex, func);
std::tuple<Explicit, void (*)()>& ra2x = ta2x;

std::tuple ta3(std::allocator_arg, alloc, 1, func, 3);
std::tuple<int, void (*)(), int>& ra3 = ta3;

std::tuple ta3x(std::allocator_arg, alloc, ex, 2, func);
std::tuple<Explicit, int, void (*)()>& ra3x = ta3x;
