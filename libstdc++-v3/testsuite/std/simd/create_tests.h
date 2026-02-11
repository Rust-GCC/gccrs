#include <stdfloat>

void create_tests()
{
  template for (auto t : {char(), short(), unsigned(), 0l, 0ull, float(), double()})
    {
      using T = decltype(t);
#ifndef EXPENSIVE_TESTS
      [[maybe_unused]] Tests<simd::vec<T>> test;
#else
      [[maybe_unused]] Tests<simd::vec<T, simd::vec<T>::size() + 3>> test0;
      [[maybe_unused]] Tests<simd::vec<T, 1>> test1;
#endif
    }
}
