// PR c++/125208
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

namespace std {
template <class _E> struct initializer_list {
  typedef _E *const_iterator;
  _E *_M_array;
  decltype (sizeof 0) _M_len;
  constexpr long size() { return _M_len; }
  constexpr const_iterator begin() { return _M_array; }
  constexpr const_iterator end() { return begin() + size(); }
};
template <typename> struct allocator_traits;
} // namespace std
constexpr void *operator new(decltype (sizeof 0), void *__p) { return __p; }
namespace std {
template <typename _Tp, typename... _Args>
constexpr void _Construct(_Tp *__p, _Args... __args) {
  new (__p) _Tp(__args...);
}
template <typename _Tp> struct __new_allocator {
  constexpr _Tp *allocate(long __n) {
    return static_cast<_Tp *>(__builtin_operator_new(__n * sizeof(_Tp)));
  }
  constexpr void deallocate(_Tp *__p, long) { __builtin_operator_delete(__p); }
};
template <typename _Tp> struct allocator_traits<__new_allocator<_Tp>> {
  using allocator_type = __new_allocator<_Tp>;
  using value_type = _Tp;
  using pointer = _Tp *;
  template <typename _Up> using rebind_alloc = __new_allocator<_Up>;
  static constexpr pointer allocate(allocator_type __a, long __n) {
    return __a.allocate(__n);
  }
  static constexpr void deallocate(allocator_type __a, pointer __p, long __n) {
    __a.deallocate(__p, __n);
  }
};
} // namespace std
template <typename _Alloc>
struct __alloc_traits : std::allocator_traits<_Alloc> {
  typedef std::allocator_traits<_Alloc> _Base_type;
  typedef _Base_type::value_type reference;
  template <typename _Tp> struct rebind {
    typedef _Base_type::template rebind_alloc<_Tp> other;
  };
};
namespace std {
template <typename _InputIterator, typename _Sentinel,
          typename _ForwardIterator>
constexpr void __do_uninit_copy(_InputIterator __first, _Sentinel __last,
                                _ForwardIterator __result) {
  for (; __first != __last; ++__first, ++__result)
    _Construct(__result, *__first);
}
template <typename _InputIterator, typename _ForwardIterator>
constexpr void uninitialized_copy(_InputIterator __first, _InputIterator __last,
                                  _ForwardIterator __result) {
  __do_uninit_copy(__first, __last, __result);
}
template <typename _InputIterator, typename _Sentinel,
          typename _ForwardIterator, typename _Tp>
constexpr void __uninitialized_copy_a(_InputIterator __first, _Sentinel __last,
                                      _ForwardIterator __result, _Tp) {
  uninitialized_copy(__first, __last, __result);
}
template <typename _Tp, typename _Alloc> struct _Vector_base {
  typedef __alloc_traits<_Alloc>::template rebind<_Tp>::other _Tp_alloc_type;
  typedef __alloc_traits<_Tp_alloc_type>::pointer pointer;
  struct _Vector_impl : _Tp_alloc_type {
    constexpr _Vector_impl(_Tp_alloc_type) {}
    pointer _M_start;
  };
  constexpr ~_Vector_base() { _M_deallocate(_M_impl._M_start); }
  _Vector_impl _M_impl;
  constexpr pointer _M_allocate(long __n) {
    return __n ? __alloc_traits<_Tp_alloc_type>::allocate(_M_impl, __n)
               : pointer();
  }
  constexpr void _M_deallocate(pointer __p) {
    __alloc_traits<_Tp_alloc_type>::deallocate(_M_impl, __p, 0);
  }
};
template <typename _Tp, typename _Alloc = __new_allocator<_Tp>>
struct vector : _Vector_base<_Tp, _Alloc> {
  typedef _Vector_base<_Tp, _Alloc> _Base;
  typedef _Base::_Tp_alloc_type _Tp_alloc_type;
  typedef _Alloc allocator_type;
  constexpr vector(initializer_list<_Tp> __l,
                   allocator_type __a = allocator_type())
      : _Base(__a) {
    long __trans_tmp_1 = __l.size();
    _M_range_initialize_n(__l.begin(), __l.end(), __trans_tmp_1);
  }
  constexpr __alloc_traits<_Tp_alloc_type>::reference operator[](long __n) {
    return *(this->_M_impl._M_start + __n);
  }
  template <typename _Iterator, typename _Sentinel>
  constexpr void _M_range_initialize_n(_Iterator __first, _Sentinel __last,
                                       long __n) {
    typename _Base::pointer __start = this->_M_allocate(__n);
    this->_M_impl._M_start = __start;
    _Tp_alloc_type __trans_tmp_2;
    __uninitialized_copy_a(__first, __last, __start, __trans_tmp_2);
  }
};
namespace meta {
using info = decltype(^^int);
struct access_context {
  info _M_scope;
  info _M_designating_class;
};
consteval vector<info> bases_of(info, access_context);
} // namespace meta
} // namespace std
template <class L> using mp_is_value_list = L;
template <template <class> class, class...> long cx_count_if;
template <std::meta::info> struct rf_base_desc;
struct X1 {};
struct X2 {};
struct Z : X1, X2 {};
int main() {
  constexpr auto all = std::meta::access_context();
  using B1 = rf_base_desc<bases_of(^^Z, all)[0]>;
  using B2 = rf_base_desc<bases_of(^^Z, all)[1]>;
  (void) cx_count_if<mp_is_value_list, B1, B2>;
}
