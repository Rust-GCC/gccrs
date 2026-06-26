/* { dg-do compile } */
/* { dg-options "-O2 -std=gnu++17" } */

template <typename> class allocator;
char copy___s2[12];
void assign(char, char &);
template <typename> struct allocator_traits;
template <typename _Tp> struct allocator_traits<allocator<_Tp>> {
  using pointer = _Tp *;
};
allocator_traits<allocator<char>>::pointer _M_p;
struct _Alloc_result {
  allocator_traits<allocator<char>>::pointer __ptr;
};
unsigned _M_string_length;
enum { _S_local_capacity = 5 } _M_allocated_capacity;
bool _M_is_local() {
  if (_M_p) {
    if (_M_string_length > _S_local_capacity)
      __builtin_unreachable();
    return true;
  }
  return false;
}
int capacity() {
  long __sz = _M_is_local() ? _S_local_capacity : _M_allocated_capacity;
  return __sz;
}
struct Trans_NS___cxx11_basic_string {
  void operator=(char) { _M_string_length = 0; }
  void operator+=(char __c) {
    _Alloc_result __trans_tmp_7;
    if (_M_string_length + 1 > capacity()) {
      char __s;
      if (_M_string_length == 1)
        assign(*__trans_tmp_7.__ptr, __s);
      __builtin_memcpy(__trans_tmp_7.__ptr, &copy___s2, _M_string_length);
      if (_M_string_length > _S_local_capacity)
        __builtin_unreachable();
    }
    assign(_M_string_length, __c);
  }
};
long build_attr_access_from_parms_specend;
void build_attr_access_from_parms() {
  Trans_NS___cxx11_basic_string spec;
  if (build_attr_access_from_parms_specend)
    spec = '+';
  spec += '[';
}
