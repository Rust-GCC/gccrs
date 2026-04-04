// { dg-do run { target c++23 } }

#include <memory>
#include <testsuite_hooks.h>

struct X { int i = 0; };

template <typename T>
  struct A : std::allocator<T>
  {
    using Base = std::allocator<T>;

    std::allocation_result<T*, size_t>
    allocate_at_least(std::size_t n)
      { return { this->Base::allocate(2*n), 2*n }; }
  };

template <typename T>
  struct M : std::allocator<T>
  {
    using Base = std::allocator<T>;
    T* allocate_at_least(size_t n) = delete;

    T* keep;
    T* allocate(std::size_t n)
      {
	keep = this->Base::allocate(n);
	return keep;
      }
  };

void base()
{
  std::allocator<X> native;
  auto a1 = native.allocate_at_least(100);
  static_assert(std::is_same_v<decltype(a1), std::allocation_result<X*>>);
  VERIFY(a1.count == 100);
  native.deallocate(a1.ptr, a1.count);

  using std_traits = std::allocator_traits<std::allocator<X>>;
  auto a2 = std_traits::allocate_at_least(native, 100);
  static_assert(std::is_same_v<decltype(a2), std::allocation_result<X*>>);
  VERIFY(a2.count == 100);
  std_traits::deallocate(native, a2.ptr, a2.count);

  A<X> custom;
  auto a3 = custom.allocate_at_least(100);
  static_assert(std::is_same_v<decltype(a3), std::allocation_result<X*>>);
  VERIFY(a3.count == 200);
  custom.deallocate(a3.ptr, a3.count);

  using custom_traits = std::allocator_traits<A<X>>;
  auto a4 = custom_traits::allocate_at_least(custom, 100);
  static_assert(std::is_same_v<decltype(a4), std::allocation_result<X*>>);
  VERIFY(a4.count == 200);
  custom_traits::deallocate(custom, a4.ptr, a4.count);

  M<X> minimal;
  using minimal_traits = std::allocator_traits<M<X>>;
  auto a5 = minimal_traits::allocate_at_least(minimal, 100);
  static_assert(std::is_same_v<decltype(a5), std::allocation_result<X*>>);
  VERIFY(a5.count == 100);
  VERIFY(a5.ptr == minimal.keep);
  minimal_traits::deallocate(minimal, a5.ptr, a5.count);
}

void extra()
{
  using SatC = std::allocator_traits<std::allocator<char>>;
  std::allocator<char> satc;
  {
    auto [p, n] = SatC::allocate_at_least(satc, 1);
    VERIFY(n == __STDCPP_DEFAULT_NEW_ALIGNMENT__);
    SatC::deallocate(satc, p, n);
  }
  {
    auto [p, n] = SatC::allocate_at_least(satc, 2);
    VERIFY(n == __STDCPP_DEFAULT_NEW_ALIGNMENT__);
    SatC::deallocate(satc, p, n);
  }
  {
    auto [p, n] =
      SatC::allocate_at_least(satc, __STDCPP_DEFAULT_NEW_ALIGNMENT__ - 1);
    VERIFY(n == __STDCPP_DEFAULT_NEW_ALIGNMENT__);
    SatC::deallocate(satc, p, n);
  }
  {
    auto [p, n] = SatC::allocate_at_least(
	satc, __STDCPP_DEFAULT_NEW_ALIGNMENT__);
    VERIFY(n == __STDCPP_DEFAULT_NEW_ALIGNMENT__);
    SatC::deallocate(satc, p, n);
  }

  using SatS = std::allocator_traits<std::allocator<short>>;
  std::allocator<short> sats;
  {
    auto [p, n] = SatS::allocate_at_least(sats, 1);
    VERIFY(n == __STDCPP_DEFAULT_NEW_ALIGNMENT__ / sizeof(short));
    SatS::deallocate(sats, p, n);
  }
  {
    auto [p, n] = SatS::allocate_at_least(sats, 2);
    VERIFY(n == __STDCPP_DEFAULT_NEW_ALIGNMENT__ / sizeof(short));
    SatS::deallocate(sats, p, n);
  }
  {
    auto [p, n] = SatS::allocate_at_least(sats,
	(__STDCPP_DEFAULT_NEW_ALIGNMENT__ - 1) / sizeof(short));
    VERIFY(n == __STDCPP_DEFAULT_NEW_ALIGNMENT__ / sizeof(short));
    SatS::deallocate(sats, p, n);
  }
  {
    auto [p, n] = SatS::allocate_at_least(sats,
	__STDCPP_DEFAULT_NEW_ALIGNMENT__ / sizeof(short));
    VERIFY(n == __STDCPP_DEFAULT_NEW_ALIGNMENT__ / sizeof(short));
    SatS::deallocate(sats, p, n);
  }

  struct A3 { char s[3]; };
  using SatA3 = std::allocator_traits<std::allocator<A3>>;
  std::allocator<A3> sata3;
  {
    auto [p, n] = SatA3::allocate_at_least(sata3, 1);
    VERIFY(n == __STDCPP_DEFAULT_NEW_ALIGNMENT__ / sizeof(A3));
    SatA3::deallocate(sata3, p, n);
  }
  {
    auto [p, n] = SatA3::allocate_at_least(sata3, 2);
    VERIFY(n == __STDCPP_DEFAULT_NEW_ALIGNMENT__ / sizeof(A3));
    SatA3::deallocate(sata3, p, n);
  }
  {
    auto [p, n] = SatA3::allocate_at_least(sata3,
	(__STDCPP_DEFAULT_NEW_ALIGNMENT__ - 1) / sizeof(A3));
    VERIFY(n == __STDCPP_DEFAULT_NEW_ALIGNMENT__ / sizeof(A3));
    SatA3::deallocate(sata3, p, n);
  }
  {
    auto [p, n] = SatA3::allocate_at_least(sata3,
	__STDCPP_DEFAULT_NEW_ALIGNMENT__ / sizeof(A3));
    VERIFY(n == __STDCPP_DEFAULT_NEW_ALIGNMENT__ / sizeof(A3));
    SatA3::deallocate(sata3, p, n);
  }

  struct Anm1 { char s[__STDCPP_DEFAULT_NEW_ALIGNMENT__ - 1]; };
  using SatAnm1 = std::allocator_traits<std::allocator<Anm1>>;
  std::allocator<Anm1> satanm1;
  {
    auto [p, n] = SatAnm1::allocate_at_least(satanm1, 1);
    VERIFY(n == 1);
    SatAnm1::deallocate(satanm1, p, n);
  }
  {
    auto [p, n] = SatAnm1::allocate_at_least(satanm1,
	__STDCPP_DEFAULT_NEW_ALIGNMENT__);
    VERIFY(n == __STDCPP_DEFAULT_NEW_ALIGNMENT__);
    SatAnm1::deallocate(satanm1, p, n);
  }
  {
    auto [p, n] = SatAnm1::allocate_at_least(satanm1,
	__STDCPP_DEFAULT_NEW_ALIGNMENT__ - 1);
    VERIFY(n == __STDCPP_DEFAULT_NEW_ALIGNMENT__);
    SatAnm1::deallocate(satanm1, p, n);
  }
}

int main()
{
  base();
  extra();
}
