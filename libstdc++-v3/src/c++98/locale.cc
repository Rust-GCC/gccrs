// Copyright (C) 1997-2023 Free Software Foundation, Inc.
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

#define _GLIBCXX_USE_CXX11_ABI 1
#include <clocale>
#include <cstring>
#include <cctype>
#include <cwctype>     // For towupper, etc.
#include <locale>
#include <ext/concurrence.h>

namespace
{
  __gnu_cxx::__mutex&
  get_locale_cache_mutex()
  {
    static __gnu_cxx::__mutex locale_cache_mutex;
    return locale_cache_mutex;
  }
} // anonymous namespace

// XXX GLIBCXX_ABI Deprecated
#ifdef _GLIBCXX_LONG_DOUBLE_COMPAT
# define _GLIBCXX_LOC_ID(mangled) extern std::locale::id mangled
_GLIBCXX_LOC_ID (_ZNSt7num_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE2idE);
_GLIBCXX_LOC_ID (_ZNSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE2idE);
_GLIBCXX_LOC_ID (_ZNSt9money_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE2idE);
_GLIBCXX_LOC_ID (_ZNSt9money_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE2idE);
# ifdef _GLIBCXX_USE_WCHAR_T
_GLIBCXX_LOC_ID (_ZNSt7num_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE2idE);
_GLIBCXX_LOC_ID (_ZNSt7num_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE2idE);
_GLIBCXX_LOC_ID (_ZNSt9money_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE2idE);
_GLIBCXX_LOC_ID (_ZNSt9money_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE2idE);
# endif
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  // Definitions for static const data members of locale.
  const locale::category 	locale::none;
  const locale::category 	locale::ctype;
  const locale::category 	locale::numeric;
  const locale::category 	locale::collate;
  const locale::category 	locale::time;
  const locale::category 	locale::monetary;
  const locale::category 	locale::messages;
  const locale::category 	locale::all;

  // These are no longer exported.
  locale::_Impl*                locale::_S_classic;
  locale::_Impl* 		locale::_S_global;

#ifdef __GTHREADS
  __gthread_once_t 		locale::_S_once = __GTHREAD_ONCE_INIT;
#endif

  locale::locale(const locale& __other) throw()
  : _M_impl(__other._M_impl)
  {
    if (_M_impl != _S_classic)
      _M_impl->_M_add_reference();
  }

  // This is used to initialize global and classic locales, and
  // assumes that the _Impl objects are constructed correctly.
  // The lack of a reference increment is intentional.
  locale::locale(_Impl* __ip) throw() : _M_impl(__ip)
  { }

  locale::~locale() throw()
  {
    if (_M_impl != _S_classic)
      _M_impl->_M_remove_reference();
  }

  bool
  locale::operator==(const locale& __rhs) const throw()
  {
    // Deal first with the common cases, fast to process: refcopies,
    // unnamed (i.e., !_M_names[0]), "simple" (!_M_names[1] => all the
    // categories same name, i.e., _M_names[0]). Otherwise fall back
    // to the general locale::name().
    bool __ret;
    if (_M_impl == __rhs._M_impl)
      __ret = true;
    else if (!_M_impl->_M_names[0] || !__rhs._M_impl->_M_names[0]
	     || std::strcmp(_M_impl->_M_names[0],
			    __rhs._M_impl->_M_names[0]) != 0)
      __ret = false;
    else if (!_M_impl->_M_names[1] && !__rhs._M_impl->_M_names[1])
      __ret = true;
    else
      __ret = this->name() == __rhs.name();
    return __ret;
  }

  const locale&
  locale::operator=(const locale& __other) throw()
  {
    if (__other._M_impl != _S_classic)
      __other._M_impl->_M_add_reference();
    if (_M_impl != _S_classic)
      _M_impl->_M_remove_reference();
    _M_impl = __other._M_impl;
    return *this;
  }

  _GLIBCXX_DEFAULT_ABI_TAG
  string
  locale::name() const
  {
    string __ret;
    if (!_M_impl->_M_names[0])
      __ret = '*';
    else if (_M_impl->_M_check_same_name())
      __ret = _M_impl->_M_names[0];
    else
      {
	__ret.reserve(128);
	__ret += _S_categories[0];
	__ret += '=';
	__ret += _M_impl->_M_names[0];
	for (size_t __i = 1; __i < _S_categories_size; ++__i)
	  {
	    __ret += ';';
	    __ret += _S_categories[__i];
	    __ret += '=';
	    __ret += _M_impl->_M_names[__i];
	  }
      }
    return __ret;
  }

  locale::category
  locale::_S_normalize_category(category __cat)
  {
    int __ret = 0;
    if (__cat == none || ((__cat & all) && !(__cat & ~all)))
      __ret = __cat;
    else
      {
	// NB: May be a C-style "LC_ALL" category; convert.
	switch (__cat)
	  {
	  case LC_COLLATE:
	    __ret = collate;
	    break;
	  case LC_CTYPE:
	    __ret = ctype;
	    break;
	  case LC_MONETARY:
	    __ret = monetary;
	    break;
	  case LC_NUMERIC:
	    __ret = numeric;
	    break;
	  case LC_TIME:
	    __ret = time;
	    break;
#ifdef _GLIBCXX_HAVE_LC_MESSAGES
	  case LC_MESSAGES:
	    __ret = messages;
	    break;
#endif
	  case LC_ALL:
	    __ret = all;
	    break;
	  default:
	    __throw_runtime_error(__N("locale::_S_normalize_category "
				  "category not found"));
	  }
      }
    return __ret;
  }

  // locale::facet
  __c_locale locale::facet::_S_c_locale;

  const char locale::facet::_S_c_name[2] = "C";

#ifdef __GTHREADS
  __gthread_once_t locale::facet::_S_once = __GTHREAD_ONCE_INIT;
#endif

  void
  locale::facet::_S_initialize_once()
  {
    // Initialize the underlying locale model.
    _S_create_c_locale(_S_c_locale, _S_c_name);
  }

  __c_locale
  locale::facet::_S_get_c_locale()
  {
#ifdef __GTHREADS
    if (__gthread_active_p())
      __gthread_once(&_S_once, _S_initialize_once);
    else
#endif
      {
	if (!_S_c_locale)
	  _S_initialize_once();
      }
    return _S_c_locale;
  }

  const char*
  locale::facet::_S_get_c_name() throw()
  { return _S_c_name; }

  locale::facet::
  ~facet() { }

  // locale::_Impl
  locale::_Impl::
  ~_Impl() throw()
  {
    if (_M_facets)
      for (size_t __i = 0; __i < _M_facets_size; ++__i)
	if (_M_facets[__i])
	  _M_facets[__i]->_M_remove_reference();
    delete [] _M_facets;

    if (_M_caches)
      for (size_t __i = 0; __i < _M_facets_size; ++__i)
	if (_M_caches[__i])
	  _M_caches[__i]->_M_remove_reference();
    delete [] _M_caches;

    if (_M_names)
      for (size_t __i = 0; __i < _S_categories_size; ++__i)
	delete [] _M_names[__i];
    delete [] _M_names;
  }

  // Clone existing _Impl object.
  locale::_Impl::
  _Impl(const _Impl& __imp, size_t __refs)
  : _M_refcount(__refs), _M_facets(0), _M_facets_size(__imp._M_facets_size),
  _M_caches(0), _M_names(0)
  {
    __try
      {
	_M_facets = new const facet*[_M_facets_size];
	for (size_t __i = 0; __i < _M_facets_size; ++__i)
	  {
	    _M_facets[__i] = __imp._M_facets[__i];
	    if (_M_facets[__i])
	      _M_facets[__i]->_M_add_reference();
	  }
	_M_caches = new const facet*[_M_facets_size];
	for (size_t __j = 0; __j < _M_facets_size; ++__j)
	  {
	    _M_caches[__j] = __imp._M_caches[__j];
	    if (_M_caches[__j])
	      _M_caches[__j]->_M_add_reference();
	  }
	_M_names = new char*[_S_categories_size];
	for (size_t __k = 0; __k < _S_categories_size; ++__k)
	  _M_names[__k] = 0;

	// Name the categories.
	for (size_t __l = 0; (__l < _S_categories_size
			      && __imp._M_names[__l]); ++__l)
	  {
	    const size_t __len = std::strlen(__imp._M_names[__l]) + 1;
	    _M_names[__l] = new char[__len];
	    std::memcpy(_M_names[__l], __imp._M_names[__l], __len);
	  }
      }
    __catch(...)
      {
	this->~_Impl();
	__throw_exception_again;
      }
  }

  void
  locale::_Impl::
  _M_replace_category(const _Impl* __imp,
		      const locale::id* const* __idpp)
  {
    for (; *__idpp; ++__idpp)
      _M_replace_facet(__imp, *__idpp);
  }

  void
  locale::_Impl::
  _M_replace_facet(const _Impl* __imp, const locale::id* __idp)
  {
    size_t __index = __idp->_M_id();
    if ((__index > (__imp->_M_facets_size - 1))
	|| !__imp->_M_facets[__index])
      __throw_runtime_error(__N("locale::_Impl::_M_replace_facet"));
    _M_install_facet(__idp, __imp->_M_facets[__index]);
  }

  void
  locale::_Impl::
  _M_install_facet(const locale::id* __idp, const facet* __fp)
  {
    if (__fp)
      {
	size_t __index = __idp->_M_id();

	// Check size of facet vector to ensure adequate room.
	if (__index > _M_facets_size - 1)
	  {
	    const size_t __new_size = __index + 4;

	    // New facet array.
	    const facet** __oldf = _M_facets;
	    const facet** __newf;
	    __newf = new const facet*[__new_size];
	    for (size_t __i = 0; __i < _M_facets_size; ++__i)
	      __newf[__i] = _M_facets[__i];
	    for (size_t __l = _M_facets_size; __l < __new_size; ++__l)
	      __newf[__l] = 0;

	    // New cache array.
	    const facet** __oldc = _M_caches;
	    const facet** __newc;
	    __try
	      {
		__newc = new const facet*[__new_size];
	      }
	    __catch(...)
	      {
		delete [] __newf;
		__throw_exception_again;
	      }
	    for (size_t __j = 0; __j < _M_facets_size; ++__j)
	      __newc[__j] = _M_caches[__j];
	    for (size_t __k = _M_facets_size; __k < __new_size; ++__k)
	      __newc[__k] = 0;

	    _M_facets_size = __new_size;
	    _M_facets = __newf;
	    _M_caches = __newc;
	    delete [] __oldf;
	    delete [] __oldc;
	  }

	__fp->_M_add_reference();
	const facet*& __fpr = _M_facets[__index];
	if (__fpr)
	  {
#if _GLIBCXX_USE_DUAL_ABI
            // If this is a twinned facet replace its twin with a shim.
            for (const id* const* p = _S_twinned_facets; *p != 0; p += 2)
              {
                if (p[0]->_M_id() == __index)
                  {
                    // replacing the old ABI facet, also replace new ABI twin
                    const facet*& __fpr2 = _M_facets[p[1]->_M_id()];
                    if (__fpr2)
                      {
                        const facet* __fp2 = __fp->_M_sso_shim(p[1]);
                        __fp2->_M_add_reference();
                        __fpr2->_M_remove_reference();
                        __fpr2 = __fp2;
                      }
                    break;
                  }
                else if (p[1]->_M_id() == __index)
                  {
                    // replacing the new ABI facet, also replace old ABI twin
                    const facet*& __fpr2 = _M_facets[p[0]->_M_id()];
                    if (__fpr2)
                      {
                        const facet* __fp2 = __fp->_M_cow_shim(p[0]);
                        __fp2->_M_add_reference();
                        __fpr2->_M_remove_reference();
                        __fpr2 = __fp2;
                      }
                    break;
                  }
              }
#endif
	    // Replacing an existing facet. Order matters.
	    __fpr->_M_remove_reference();
	    __fpr = __fp;
	  }
	else
	  {
	    // Installing a newly created facet into an empty
	    // _M_facets container, say a newly-constructed,
	    // swanky-fresh _Impl.
	    _M_facets[__index] = __fp;
	  }

	// Ideally, it would be nice to only remove the caches that
	// are now incorrect. However, some of the caches depend on
	// multiple facets, and we only know about one facet
	// here. It's no great loss: the first use of the new facet
	// will create a new, correctly cached facet anyway.
	for (size_t __i = 0; __i < _M_facets_size; ++__i)
	  {
	    const facet* __cpr = _M_caches[__i];
	    if (__cpr)
	      {
		__cpr->_M_remove_reference();
		_M_caches[__i] = 0;
	      }
	  }
      }
  }

  void
  locale::_Impl::
  _M_install_cache(const facet* __cache, size_t __index)
  {
    __gnu_cxx::__scoped_lock sentry(get_locale_cache_mutex());
#if _GLIBCXX_USE_DUAL_ABI
    // If this cache is for one of the facets that is instantiated twice,
    // for old and new std::string ABI, install it in both slots.
    size_t __index2 = -1;
    for (const id* const* p = _S_twinned_facets; *p != 0; p += 2)
      {
        if (p[0]->_M_id() == __index)
          {
            __index2 = p[1]->_M_id();
            break;
          }
        else if (p[1]->_M_id() == __index)
          {
            __index2 = __index;
            __index = p[0]->_M_id();
            break;
          }
      }
#endif
    if (_M_caches[__index] != 0)
      {
	// Some other thread got in first.
	delete __cache;
      }
    else
      {
	__cache->_M_add_reference();
	_M_caches[__index] = __cache;
#if _GLIBCXX_USE_DUAL_ABI
        if (__index2 != size_t(-1))
          {
            __cache->_M_add_reference();
            _M_caches[__index2] = __cache;
          }
#endif
      }
  }

  // locale::id
  // Definitions for static const data members of locale::id
  _Atomic_word locale::id::_S_refcount;  // init'd to 0 by linker

  // XXX GLIBCXX_ABI Deprecated
#ifdef _GLIBCXX_LONG_DOUBLE_COMPAT
namespace {
  inline locale::id*
  find_ldbl_sync_facet(const locale::id* __idp)
  {
# define _GLIBCXX_SYNC_ID(facet, mangled) \
    if (__idp == &::mangled)		  \
      return &facet::id

    _GLIBCXX_SYNC_ID (num_get<char>, _ZNSt7num_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE2idE);
    _GLIBCXX_SYNC_ID (num_put<char>, _ZNSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE2idE);
    _GLIBCXX_SYNC_ID (money_get<char>, _ZNSt9money_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE2idE);
    _GLIBCXX_SYNC_ID (money_put<char>, _ZNSt9money_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE2idE);
# ifdef _GLIBCXX_USE_WCHAR_T
    _GLIBCXX_SYNC_ID (num_get<wchar_t>, _ZNSt7num_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE2idE);
    _GLIBCXX_SYNC_ID (num_put<wchar_t>, _ZNSt7num_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE2idE);
    _GLIBCXX_SYNC_ID (money_get<wchar_t>, _ZNSt9money_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE2idE);
    _GLIBCXX_SYNC_ID (money_put<wchar_t>, _ZNSt9money_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE2idE);
# endif
    return 0;
  }
} // namespace
#endif

  size_t
  locale::id::_M_id() const throw()
  {
    if (!_M_index)
      {
	// XXX GLIBCXX_ABI Deprecated
#ifdef _GLIBCXX_LONG_DOUBLE_COMPAT
	if (locale::id* f = find_ldbl_sync_facet(this))
	{
	  const size_t sync_id = f->_M_id();
	  _M_index = 1 + sync_id;
	  return sync_id;
	}
#endif

#ifdef __GTHREADS
	if (!__gnu_cxx::__is_single_threaded())
	  {
	    if (__atomic_always_lock_free(sizeof(_M_index), &_M_index))
	      {
		const _Atomic_word next
		  = 1 + __gnu_cxx::__exchange_and_add(&_S_refcount, 1);
		size_t expected = 0;
		__atomic_compare_exchange_n(&_M_index, &expected, next,
					    /* weak = */ false,
					    /* success = */ __ATOMIC_ACQ_REL,
					    /* failure = */ __ATOMIC_ACQUIRE);
	      }
	    else
	      {
		static __gnu_cxx::__mutex m;
		__gnu_cxx::__scoped_lock l(m);
		if (!_M_index)
		  _M_index = ++_S_refcount;
	      }
	  }
	else
#endif
	_M_index = ++_S_refcount; // single-threaded case
      }
    return _M_index - 1;
  }

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace
