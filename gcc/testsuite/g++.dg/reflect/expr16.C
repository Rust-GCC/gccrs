// PR c++/124575
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

constexpr const decltype(^^::) &r = ^^int;
