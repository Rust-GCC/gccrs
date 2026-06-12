// PR c++/125770
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

consteval {
    members_of(^^::, std::meta::access_context::unchecked());
}
