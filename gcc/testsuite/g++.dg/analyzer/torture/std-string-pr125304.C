/* Reproduced from libstdc++ at r17-368-gb1cd9bd9908b0f
   with assertions disabled.  */

/* { dg-skip-if "" { *-*-* } { "-fno-fat-lto-objects" } { "" } } */

void __analyzer_eval(int);
namespace std {
template <class> struct char_traits;
template <typename _Tp> struct remove_reference {
  using type = _Tp;
};
bool is_constant_evaluated() { return 0; }

template <typename _Tp> remove_reference<_Tp>::type move(_Tp);
template <typename _Tp> _Tp *addressof(_Tp &__r) {
  return __builtin_addressof(__r);
}
template <typename _Tp> struct __ptr_traits_ptr_to {
  static _Tp *pointer_to(_Tp &__r) { return addressof(__r); }
};
template <typename> struct pointer_traits;
template <typename _Tp>
struct pointer_traits<_Tp *> : __ptr_traits_ptr_to<_Tp> {};
static void assign(char &__c1, const char &__c2) { __c1 = __c2; }
template <> struct char_traits<char> {};
template <typename> struct allocator {
  void deallocate(char *__p, long) { __builtin_operator_delete(__p); }
};
template <typename> struct allocator_traits;
using size_type = __SIZE_TYPE__;
template <typename _Tp> struct allocator_traits<allocator<_Tp>> {
  using pointer = _Tp *;
  using const_pointer = const _Tp *;
  static void deallocate(allocator<_Tp> __a, pointer __p, size_type __n) {
    __a.deallocate(__p, __n);
  }
};
} // namespace std
struct __alloc_traits : std::allocator_traits<std::allocator<char>> {};
namespace std {
template <typename _CharT, typename, typename _Alloc> struct basic_string {
  typedef __alloc_traits _Alloc_traits;
  typedef char_traits<char> traits_type;
  typedef _Alloc allocator_type;
  typedef _Alloc_traits::pointer pointer;
  typedef _Alloc_traits::const_pointer const_pointer;
  // Keep everything from here onwards
  struct _Alloc_hider : allocator_type {
    _Alloc_hider(pointer __dat, _Alloc &&__a = _Alloc())
        : allocator_type(std::move(__a)), _M_p(__dat) {}
    pointer _M_p;
  };
  _Alloc_hider _M_dataplus;
  size_type _M_string_length;
  enum { _S_local_capacity = 15 / sizeof(_CharT) };
  union {
    _CharT _M_local_buf[_S_local_capacity + 1];
    size_type _M_allocated_capacity;
  };
  void _M_length(size_type __length) { _M_string_length = __length; }
  pointer _M_data() const { return _M_dataplus._M_p; }
  pointer _M_local_data() {
    return std::pointer_traits<pointer>::pointer_to(*_M_local_buf);
  }
  const_pointer _M_local_data() const {
    return std::pointer_traits<const_pointer>::pointer_to(*_M_local_buf);
  }
  void _M_set_length(size_type __n) {
    assign(_M_data()[__n], _CharT());
    _M_length(__n);
  }

public:
  bool _M_is_local() const {
    if (_M_data() == _M_local_data()) {
      if (_M_string_length > _S_local_capacity)
        __builtin_unreachable();
      return true;
    }
    return false;
  }

private:
  void _M_dispose() {
    if (!_M_is_local())
      _M_destroy(_M_allocated_capacity);
  }
  void _M_destroy(size_type __size) throw() {
    _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1);
  };
  ;
  ;
  allocator_type &_M_get_allocator();
  __attribute__((__always_inline__)) void _M_init_local_buf() noexcept {
    if (std::is_constant_evaluated())
      for (size_type __i = 0; __i <= _S_local_capacity; ++__i)
        _M_local_buf[__i] = _CharT();
  }

public:
  basic_string() : _M_dataplus(_M_local_data()) {
    _M_init_local_buf();
    _M_set_length(0);
  };
  ~basic_string() { _M_dispose(); }
  size_type size() const noexcept {
    size_type __sz = _M_string_length;
    return __sz;
  }
  size_type length() const noexcept { return size(); }
};

typedef basic_string<char, char_traits<char>, allocator<char>> string;

} // namespace std

void test_ctor_empty() {
  std::string s;
  __analyzer_eval(s.length() == 0); // { dg-warning "TRUE" }
  __analyzer_eval(s.size() == 0);   // { dg-warning "TRUE" }
  __analyzer_eval(s._M_is_local()); // { dg-warning "TRUE" }
}
