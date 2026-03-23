// { dg-do run { target c++26 } }
// { dg-require-atomic-cmpxchg-word "" }
// { dg-add-options libatomic }

#include <atomic>
#include <memory>
#include <type_traits>

#include <testsuite_hooks.h>

template <typename T, typename CVV>
void testAtomicRefAddress()
{
  T x(T(42));
  const std::atomic_ref<T> a(x);

  static_assert( noexcept(a.address()) );
  static_assert( std::is_same_v<decltype(a.address()), CVV*> );
  VERIFY( std::addressof(x) == a.address() );
}

template <typename T>
void testAtomicRefAddressForCV()
{
  testAtomicRefAddress<T, void>();
  testAtomicRefAddress<const T, const void>();
  if constexpr (std::atomic_ref<T>::is_always_lock_free)
  {
    testAtomicRefAddress<volatile T, volatile void>();
    testAtomicRefAddress<const volatile T, const volatile void>();
  }
}

int
main ()
{
  struct X { int c; };
  testAtomicRefAddressForCV<X>();
  testAtomicRefAddressForCV<int>();
  testAtomicRefAddressForCV<float>();
  testAtomicRefAddressForCV<char*>();
}
