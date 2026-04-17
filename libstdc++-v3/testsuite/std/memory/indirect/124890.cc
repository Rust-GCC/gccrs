// { dg-do compile { target c++26 } }

// PR libstdc++/124890 - Circular constraint in std::indirect::operator==

#include <memory>
#include <vector>

using I = std::vector<std::indirect<int>>::iterator;
static_assert(std::totally_ordered<I>);
