// PR c++/124477
// { dg-additional-options "-fmodules -Wno-global-module -O3" }
// { dg-module-cmi M }

module;
extern "C" {
  typedef __SIZE_TYPE__ size_t;

  extern void *memset (void *__s, int __c, size_t __n) noexcept (true) __attribute__ ((__nonnull__ (1)));

  extern
    __inline
    __attribute__((__always_inline__))
    __attribute__((__gnu_inline__))
    __attribute__((__artificial__))
  void *
    __attribute__((__leaf__))
  memset(void *__dest, int __ch, size_t __len) noexcept(true) {
    return __builtin___memset_chk(__dest, __ch, __len,
                                  __builtin_object_size(__dest, 0));
  }
}
export module M;
export using ::memset;
