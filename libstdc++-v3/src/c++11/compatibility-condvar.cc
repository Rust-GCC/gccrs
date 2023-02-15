// Compatibility symbols for previous versions, C++0x bits -*- C++ -*-

// Copyright (C) 2013-2023 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

#include <bits/c++config.h>

#if __cplusplus < 201103L
# error "compatibility-condvar-c++0x.cc must be compiled with -std=gnu++11"
#endif

#if _GLIBCXX_INLINE_VERSION
# error "compatibility-thread-c++0x.cc is not needed for gnu-versioned-namespace"
#endif

#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)

#define condition_variable_any condition_variable_anyXX
#include <condition_variable>
#undef condition_variable_any

// XXX GLIBCXX_ABI Deprecated
// gcc-4.9.0
// std::condition_variable_any replaced with std::_V2::condition_variable_any
namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
  class condition_variable_any
  {
    condition_variable			_M_cond;
    mutex				_M_mutex;

  public:
    condition_variable_any() noexcept;
    ~condition_variable_any() noexcept;
  };
  condition_variable_any::condition_variable_any() noexcept = default;
  condition_variable_any::~condition_variable_any() noexcept = default;
_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#if ! _GLIBCXX_INLINE_VERSION
// XXX GLIBCXX_ABI Deprecated
// gcc-12.1
// std::condition_variable::wait changed to noexcept(false)
#if defined(_GLIBCXX_SYMVER_GNU) && defined(_GLIBCXX_SHARED) \
    && defined(_GLIBCXX_HAVE_AS_SYMVER_DIRECTIVE) \
    && defined(_GLIBCXX_HAVE_SYMVER_SYMBOL_RENAMING_RUNTIME_SUPPORT)
namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
namespace
{
  // Pointer-to-member for private std::condition_variable::_M_cond member.
  std::__condvar std::condition_variable::* __base_member;

  template<std::__condvar std::condition_variable::*X>
    struct cracker
    { static std::__condvar std::condition_variable::* value; };

  // Initializer for this static member also initializes __base_member.
  template<std::__condvar std::condition_variable::*X>
    std::__condvar std::condition_variable::*
      cracker<X>::value = __base_member = X;

  // Explicit instantiation is allowed to access the private member.
  template class cracker<&std::condition_variable::_M_cond>;
}

struct __nothrow_wait_cv : std::condition_variable
{
  void wait(std::unique_lock<std::mutex>&) noexcept;
};

__attribute__((used))
void
__nothrow_wait_cv::wait(std::unique_lock<std::mutex>& lock) noexcept
{
  // In theory this could be simply this->std::condition_variable::wait(lock)
  // but with uclibc that binds to the @GLIBCXX_3.4.11 symbol, see PR 105730.
  (this->*__base_member).wait(*lock.mutex());
}
} // namespace __gnu_cxx

// Export a noexcept wrapper around std::condition_variable::wait
// with the original @GLIBCXX_3.4.11 symbol version.
asm(
  ".symver _ZN9__gnu_cxx17__nothrow_wait_cv4waitERSt11unique_lockISt5mutexE,"
  "_ZNSt18condition_variable4waitERSt11unique_lockISt5mutexE@GLIBCXX_3.4.11"
);
#endif
#endif

#endif // _GLIBCXX_HAS_GTHREADS && _GLIBCXX_USE_C99_STDINT_TR1
