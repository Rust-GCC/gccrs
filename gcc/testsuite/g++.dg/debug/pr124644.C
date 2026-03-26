// { dg-do compile }
// { dg-options "-O -w -g1 -fchecking" }

template <int __v> struct integral_constant {
  static constexpr int value = __v;
};
struct __conjunction_impl {
  using type = integral_constant<1>;
};
template <bool, typename _Iftrue, typename> using conditional_t = _Iftrue;
template <int> struct _Fixed;
template <int> struct _VecBuiltin;
template <int _Np> using fixed_size = _Fixed<_Np>;
template <typename...> struct _SimdTuple;
template <int> struct __fixed_size_storage;
template <typename, int _Np>
using __fixed_size_storage_t = __fixed_size_storage<_Np>::type;
struct _SimdWrapperBase {
  [[__gnu__::__artificial__]] constexpr _SimdWrapperBase(int);
};
struct _SimdWrapper : _SimdWrapperBase {
  using _SimdWrapperBase::_SimdWrapperBase;
};
template <unsigned long> struct __deduce_impl;
template <typename, long _Np> using deduce_t = __deduce_impl<_Np>::type;
template <template <int> class _Abi, int _Bytes> struct __find_next_valid_abi {
  static auto _S_choose() { return _Abi<_Bytes>(); }
  using type = decltype(_S_choose());
};
template <template <int> class...> struct _AbiList;
const int _S_determine_best_abi__Bytes = sizeof(int);
template <template <int> class _A0, template <int> class... _Rest>
struct _AbiList<_A0, _Rest...> {
  template <typename, int> static auto _S_determine_best_abi() {
    return typename __find_next_valid_abi<_A0,
                                          _S_determine_best_abi__Bytes>::type{};
  }
  template <typename _Tp, int _Np>
  using _BestAbi = decltype(_S_determine_best_abi<_Tp, _Np>());
};
template <typename _Abi> struct _SimdTraits : _Abi::__traits<int> {};
template <int _Np> struct __deduce_fixed_size_fallback {
  using type = fixed_size<_Np>;
};
template <unsigned long _Np>
struct __deduce_impl : __deduce_fixed_size_fallback<_Np> {};
template <typename _Abi> struct simd {
  using _Traits = _SimdTraits<_Abi>;
  using _Impl = _Traits::_SimdImpl;
  using abi_type = _Abi;
  template <typename _Up> simd(_Up __x) : _M_data(_Impl::_S_broadcast(__x)) {}
  friend void operator!=(simd, simd);
  _Traits::_SimdMember _M_data;
};
struct _SimdTupleData {
  _SimdWrapper second;
};
template <typename _Tp, typename _Abi0, typename... _Abis>
struct _SimdTuple<_Tp, _Abi0, _Abis...> : _SimdTupleData {
  template <typename _Fp> static _SimdTuple _S_generate(_Fp) { return {0}; }
};
template <typename _Tp, int _Np, typename,
          typename = simd<_AbiList<_VecBuiltin>::_BestAbi<_Tp, _Np>>>
struct __fixed_size_storage_builder;
template <int _Np>
struct __fixed_size_storage
    : __fixed_size_storage_builder<long, _Np, _SimdTuple<>> {};
template <typename _Tp, int _Np, typename... _As, typename _Next>
struct __fixed_size_storage_builder<_Tp, _Np, _SimdTuple<_As...>, _Next> {
  using type = _SimdTuple<_Tp, typename _Next::abi_type>;
};
template <int _Np> struct _SimdImplFixedSize {
  template <typename _Tp> using _SimdMember = __fixed_size_storage_t<_Tp, _Np>;
  template <typename _Tp> static _SimdMember<_Tp> _S_broadcast(_Tp) {
    return _SimdMember<_Tp>::_S_generate([] {});
  }
};
template <int _Np> struct _Fixed {
  template <typename> struct __traits {
    using _SimdImpl = _SimdImplFixedSize<_Np>;
    using _SimdMember = __fixed_size_storage_t<int, _Np>;
  };
};
struct _GnuTraits {
  using _SimdImpl = long;
  using _SimdMember = _SimdWrapper;
};
template <int> struct _VecBuiltin {
  template <typename>
  static constexpr bool _S_is_valid_v = __conjunction_impl::type ::value;
  template <typename _Tp>
  using __traits = conditional_t<_S_is_valid_v<_Tp>, _GnuTraits, int>;
};
template <typename U, int N> using V = simd<deduce_t<U, N>>;
void reduce(V<long, 4> x) { x != 1; }
