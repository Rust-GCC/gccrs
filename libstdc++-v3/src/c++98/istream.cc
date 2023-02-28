// Input streams -*- C++ -*-

// Copyright (C) 2004-2023 Free Software Foundation, Inc.
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

//
// ISO C++ 14882: 27.6.1  Input streams
//

#include <istream>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template<>
    basic_istream<char>&
    basic_istream<char>::
    getline(char_type* __s, streamsize __n, char_type __delim)
    {
      _M_gcount = 0;
      ios_base::iostate __err = ios_base::goodbit;
      sentry __cerb(*this, true);
      if (__cerb)
	{
          __try
	    {
	      const int_type __idelim = traits_type::to_int_type(__delim);
	      const int_type __eof = traits_type::eof();
	      __streambuf_type* __sb = this->rdbuf();
	      int_type __c = __sb->sgetc();

	      while (_M_gcount + 1 < __n
		     && !traits_type::eq_int_type(__c, __eof)
		     && !traits_type::eq_int_type(__c, __idelim))
		{
		  streamsize __size = std::min(streamsize(__sb->egptr()
							  - __sb->gptr()),
					       streamsize(__n - _M_gcount
							  - 1));
		  if (__size > 1)
		    {
		      const char_type* __p = traits_type::find(__sb->gptr(),
							       __size,
							       __delim);
		      if (__p)
			__size = __p - __sb->gptr();
		      traits_type::copy(__s, __sb->gptr(), __size);
		      __s += __size;
		      __sb->__safe_gbump(__size);
		      _M_gcount += __size;
		      __c = __sb->sgetc();
		    }
		  else
		    {
		      *__s++ = traits_type::to_char_type(__c);
		      ++_M_gcount;
		      __c = __sb->snextc();
		    }
		}

	      if (traits_type::eq_int_type(__c, __eof))
		__err |= ios_base::eofbit;
	      else if (traits_type::eq_int_type(__c, __idelim))
		{
		  ++_M_gcount;
		  __sb->sbumpc();
		}
	      else
		__err |= ios_base::failbit;
	    }
	  __catch(__cxxabiv1::__forced_unwind&)
	    {
	      this->_M_setstate(ios_base::badbit);
	      __throw_exception_again;
	    }
	  __catch(...)
	    { this->_M_setstate(ios_base::badbit); }
	}
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 243. get and getline when sentry reports failure.
      if (__n > 0)
	*__s = char_type();
      if (!_M_gcount)
	__err |= ios_base::failbit;
      if (__err)
	this->setstate(__err);
      return *this;
    }

  template<>
    basic_istream<char>&
    basic_istream<char>::
    ignore(streamsize __n, int_type __delim)
    {
      if (traits_type::eq_int_type(__delim, traits_type::eof()))
	return ignore(__n);

      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__n > 0 && __cerb)
	{
	  ios_base::iostate __err = ios_base::goodbit;
	  __try
	    {
	      const char_type __cdelim = traits_type::to_char_type(__delim);
	      const int_type __eof = traits_type::eof();
	      __streambuf_type* __sb = this->rdbuf();
	      int_type __c = __sb->sgetc();

	      bool __large_ignore = false;
	      while (true)
		{
		  while (_M_gcount < __n
			 && !traits_type::eq_int_type(__c, __eof)
			 && !traits_type::eq_int_type(__c, __delim))
		    {
		      streamsize __size = std::min(streamsize(__sb->egptr()
							      - __sb->gptr()),
						   streamsize(__n - _M_gcount));
		      if (__size > 1)
			{
			  const char_type* __p = traits_type::find(__sb->gptr(),
								   __size,
								   __cdelim);
			  if (__p)
			    __size = __p - __sb->gptr();
			  __sb->__safe_gbump(__size);
			  _M_gcount += __size;
			  __c = __sb->sgetc();
			}
		      else
			{
			  ++_M_gcount;
			  __c = __sb->snextc();
			}
		    }
		  if (__n == __gnu_cxx::__numeric_traits<streamsize>::__max
		      && !traits_type::eq_int_type(__c, __eof)
		      && !traits_type::eq_int_type(__c, __delim))
		    {
		      _M_gcount =
			__gnu_cxx::__numeric_traits<streamsize>::__min;
		      __large_ignore = true;
		    }
		  else
		    break;
		}

	      if (__n == __gnu_cxx::__numeric_traits<streamsize>::__max)
		{
		  if (__large_ignore)
		    _M_gcount = __gnu_cxx::__numeric_traits<streamsize>::__max;

		  if (traits_type::eq_int_type(__c, __eof))
		    __err |= ios_base::eofbit;
		  else
		    {
		      if (_M_gcount != __n)
			++_M_gcount;
		      __sb->sbumpc();
		    }
		}
	      else if (_M_gcount < __n) // implies __c == __delim or EOF
		{
		  if (traits_type::eq_int_type(__c, __eof))
		    __err |= ios_base::eofbit;
		  else
		    {
		      ++_M_gcount;
		      __sb->sbumpc();
		    }
		}
	    }
	  __catch(__cxxabiv1::__forced_unwind&)
	    {
	      this->_M_setstate(ios_base::badbit);
	      __throw_exception_again;
	    }
	  __catch(...)
	    { this->_M_setstate(ios_base::badbit); }
	  if (__err)
	    this->setstate(__err);
	}
      return *this;
    }

    void
    __istream_extract(istream& __in, char* __s, streamsize __num)
    {
      typedef basic_istream<char>       	__istream_type;
      typedef __istream_type::int_type		__int_type;
      typedef __istream_type::char_type		__char_type;
      typedef __istream_type::traits_type	__traits_type;
      typedef __istream_type::__streambuf_type  __streambuf_type;
      typedef __istream_type::__ctype_type	__ctype_type;

      streamsize __extracted = 0;
      ios_base::iostate __err = ios_base::goodbit;
      __istream_type::sentry __cerb(__in, false);
      if (__cerb)
	{
	  __try
	    {
	      // Figure out how many characters to extract.
	      streamsize __width = __in.width();
	      if (0 < __width && __width < __num)
		__num = __width;

	      const __ctype_type& __ct = use_facet<__ctype_type>(__in.getloc());

	      const __int_type __eof = __traits_type::eof();
	      __streambuf_type* __sb = __in.rdbuf();
	      __int_type __c = __sb->sgetc();

	      while (__extracted < __num - 1
		     && !__traits_type::eq_int_type(__c, __eof)
		     && !__ct.is(ctype_base::space,
				 __traits_type::to_char_type(__c)))
		{
		  streamsize __size = std::min(streamsize(__sb->egptr()
							  - __sb->gptr()),
					       streamsize(__num - __extracted
							  - 1));
		  if (__size > 1)
		    {
		      __size = (__ct.scan_is(ctype_base::space,
					     __sb->gptr() + 1,
					     __sb->gptr() + __size)
				- __sb->gptr());
		      __traits_type::copy(__s, __sb->gptr(), __size);
		      __s += __size;
		      __sb->__safe_gbump(__size);
		      __extracted += __size;
		      __c = __sb->sgetc();
		    }
		  else
		    {
		      *__s++ = __traits_type::to_char_type(__c);
		      ++__extracted;
		      __c = __sb->snextc();
		    }
		}

	      if (__extracted < __num - 1
		  && __traits_type::eq_int_type(__c, __eof))
		__err |= ios_base::eofbit;

	      // _GLIBCXX_RESOLVE_LIB_DEFECTS
	      // 68.  Extractors for char* should store null at end
	      *__s = __char_type();
	      __in.width(0);
	    }
	  __catch(__cxxabiv1::__forced_unwind&)
	    {
	      __in._M_setstate(ios_base::badbit);
	      __throw_exception_again;
	    }
	  __catch(...)
	    { __in._M_setstate(ios_base::badbit); }
	}
      if (!__extracted)
	__err |= ios_base::failbit;
      if (__err)
	__in.setstate(__err);
    }

#ifdef _GLIBCXX_USE_WCHAR_T
  template<>
    basic_istream<wchar_t>&
    basic_istream<wchar_t>::
    getline(char_type* __s, streamsize __n, char_type __delim)
    {
      _M_gcount = 0;
      ios_base::iostate __err = ios_base::goodbit;
      sentry __cerb(*this, true);
      if (__cerb)
	{
          __try
	    {
	      const int_type __idelim = traits_type::to_int_type(__delim);
	      const int_type __eof = traits_type::eof();
	      __streambuf_type* __sb = this->rdbuf();
	      int_type __c = __sb->sgetc();

	      while (_M_gcount + 1 < __n
		     && !traits_type::eq_int_type(__c, __eof)
		     && !traits_type::eq_int_type(__c, __idelim))
		{
		  streamsize __size = std::min(streamsize(__sb->egptr()
							  - __sb->gptr()),
					       streamsize(__n - _M_gcount
							  - 1));
		  if (__size > 1)
		    {
		      const char_type* __p = traits_type::find(__sb->gptr(),
							       __size,
							       __delim);
		      if (__p)
			__size = __p - __sb->gptr();
		      traits_type::copy(__s, __sb->gptr(), __size);
		      __s += __size;
		      __sb->__safe_gbump(__size);
		      _M_gcount += __size;
		      __c = __sb->sgetc();
		    }
		  else
		    {
		      *__s++ = traits_type::to_char_type(__c);
		      ++_M_gcount;
		      __c = __sb->snextc();
		    }
		}

	      if (traits_type::eq_int_type(__c, __eof))
		__err |= ios_base::eofbit;
	      else if (traits_type::eq_int_type(__c, __idelim))
		{
		  ++_M_gcount;
		  __sb->sbumpc();
		}
	      else
		__err |= ios_base::failbit;
	    }
	  __catch(__cxxabiv1::__forced_unwind&)
	    {
	      this->_M_setstate(ios_base::badbit);
	      __throw_exception_again;
	    }
	  __catch(...)
	    { this->_M_setstate(ios_base::badbit); }
	}
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 243. get and getline when sentry reports failure.
      if (__n > 0)
	*__s = char_type();
      if (!_M_gcount)
	__err |= ios_base::failbit;
      if (__err)
	this->setstate(__err);
      return *this;
    }

  template<>
    basic_istream<wchar_t>&
    basic_istream<wchar_t>::
    ignore(streamsize __n, int_type __delim)
    {
      if (traits_type::eq_int_type(__delim, traits_type::eof()))
	return ignore(__n);

      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__n > 0 && __cerb)
	{
	  ios_base::iostate __err = ios_base::goodbit;
	  __try
	    {
	      const char_type __cdelim = traits_type::to_char_type(__delim);
	      const int_type __eof = traits_type::eof();
	      __streambuf_type* __sb = this->rdbuf();
	      int_type __c = __sb->sgetc();

	      bool __large_ignore = false;
	      while (true)
		{
		  while (_M_gcount < __n
			 && !traits_type::eq_int_type(__c, __eof)
			 && !traits_type::eq_int_type(__c, __delim))
		    {
		      streamsize __size = std::min(streamsize(__sb->egptr()
							      - __sb->gptr()),
						   streamsize(__n - _M_gcount));
		      if (__size > 1)
			{
			  const char_type* __p = traits_type::find(__sb->gptr(),
								   __size,
								   __cdelim);
			  if (__p)
			    __size = __p - __sb->gptr();
			  __sb->__safe_gbump(__size);
			  _M_gcount += __size;
			  __c = __sb->sgetc();
			}
		      else
			{
			  ++_M_gcount;
			  __c = __sb->snextc();
			}
		    }
		  if (__n == __gnu_cxx::__numeric_traits<streamsize>::__max
		      && !traits_type::eq_int_type(__c, __eof)
		      && !traits_type::eq_int_type(__c, __delim))
		    {
		      _M_gcount =
			__gnu_cxx::__numeric_traits<streamsize>::__min;
		      __large_ignore = true;
		    }
		  else
		    break;
		}

	      if (__n == __gnu_cxx::__numeric_traits<streamsize>::__max)
		{
		  if (__large_ignore)
		    _M_gcount = __gnu_cxx::__numeric_traits<streamsize>::__max;

		  if (traits_type::eq_int_type(__c, __eof))
		    __err |= ios_base::eofbit;
		  else
		    {
		      if (_M_gcount != __n)
			++_M_gcount;
		      __sb->sbumpc();
		    }
		}
	      else if (_M_gcount < __n) // implies __c == __delim or EOF
		{
		  if (traits_type::eq_int_type(__c, __eof))
		    __err |= ios_base::eofbit;
		  else
		    {
		      ++_M_gcount;
		      __sb->sbumpc();
		    }
		}
	    }
	  __catch(__cxxabiv1::__forced_unwind&)
	    {
	      this->_M_setstate(ios_base::badbit);
	      __throw_exception_again;
	    }
	  __catch(...)
	    { this->_M_setstate(ios_base::badbit); }
	  if (__err)
	    this->setstate(__err);
	}
      return *this;
    }
#endif


_GLIBCXX_END_NAMESPACE_VERSION
} // namespace
