// -*- C++ -*-
// Iterator Wrappers for the C++ library testsuite.
//
// Copyright (C) 2004-2019 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.
//

// This file provides the following:
//
// input_iterator_wrapper, output_iterator_wrapper
// forward_iterator_wrapper, bidirectional_iterator_wrapper and
// random_access_wrapper, which attempt to exactly perform the requirements
// of these types of iterators. These are constructed from the class
// test_container, which is given two pointers to T and an iterator type.

#include <testsuite_hooks.h>
#include <bits/stl_iterator_base_types.h>

#if __cplusplus >= 201103L
#include <bits/move.h>
#endif

#ifndef _TESTSUITE_ITERATORS
#define _TESTSUITE_ITERATORS

#ifdef DISABLE_ITERATOR_DEBUG
#define ITERATOR_VERIFY(x)
#else
#define ITERATOR_VERIFY(x) VERIFY(x)
#endif

namespace __gnu_test
{
  /**
   * @brief Simple container for holding two pointers.
   *
   * Note that input_iterator_wrapper changes first to denote
   * how the valid range of == , ++, etc. change as the iterators are used.
   */
  template<typename T>
    struct BoundsContainer
    {
      T* first;
      T* last;

      BoundsContainer(T* _first, T* _last) : first(_first), last(_last)
      { }

      std::size_t size() const { return last - first; }
    };

  // Simple container for holding state of a set of output iterators.
  template<typename T>
    struct OutputContainer : public BoundsContainer<T>
    {
      T* incrementedto;
      bool* writtento;

      OutputContainer(T* _first, T* _last)
      : BoundsContainer<T>(_first, _last), incrementedto(_first),
	writtento(new bool[this->size()]())
      { }

      ~OutputContainer()
      { delete[] writtento; }
    };

  // Produced by output_iterator to allow limited writing to pointer
  template<class T>
    class WritableObject
    {
      T* ptr;

    public:
      OutputContainer<T>* SharedInfo;

      WritableObject(T* ptr_in, OutputContainer<T>* SharedInfo_in):
	ptr(ptr_in), SharedInfo(SharedInfo_in)
      { }

#if __cplusplus >= 201103L
      template<class U>
      typename std::enable_if<std::is_assignable<T&, U>::value>::type
      operator=(U&& new_val) const
      {
	ITERATOR_VERIFY(SharedInfo->writtento[ptr - SharedInfo->first] == 0);
	SharedInfo->writtento[ptr - SharedInfo->first] = 1;
	*ptr = std::forward<U>(new_val);
      }
#else
      template<class U>
      void
      operator=(const U& new_val)
      {
	ITERATOR_VERIFY(SharedInfo->writtento[ptr - SharedInfo->first] == 0);
	SharedInfo->writtento[ptr - SharedInfo->first] = 1;
	*ptr = new_val;
      }
#endif
    };

  /**
   * @brief output_iterator wrapper for pointer
   *
   * This class takes a pointer and wraps it to provide exactly
   * the requirements of a output_iterator. It should not be
   * instantiated directly, but generated from a test_container
   */
  template<class T>
  struct output_iterator_wrapper
  : public std::iterator<std::output_iterator_tag, T, std::ptrdiff_t, T*, T&>
  {
    typedef OutputContainer<T> ContainerType;
    T* ptr;
    ContainerType* SharedInfo;

    output_iterator_wrapper(T* _ptr, ContainerType* SharedInfo_in)
    : ptr(_ptr), SharedInfo(SharedInfo_in)
    {
      ITERATOR_VERIFY(ptr >= SharedInfo->first && ptr <= SharedInfo->last);
    }

#if __cplusplus >= 201103L
    output_iterator_wrapper() = delete;

    output_iterator_wrapper(const output_iterator_wrapper&) = default;

    output_iterator_wrapper&
    operator=(const output_iterator_wrapper&) = default;
#endif

    WritableObject<T>
    operator*() const
    {
      ITERATOR_VERIFY(ptr < SharedInfo->last);
      ITERATOR_VERIFY(SharedInfo->writtento[ptr - SharedInfo->first] == false);
      return WritableObject<T>(ptr, SharedInfo);
    }

    output_iterator_wrapper&
    operator++()
    {
      ITERATOR_VERIFY(SharedInfo && ptr < SharedInfo->last);
      ITERATOR_VERIFY(ptr>=SharedInfo->incrementedto);
      ptr++;
      SharedInfo->incrementedto=ptr;
      return *this;
    }

    output_iterator_wrapper
    operator++(int)
    {
      output_iterator_wrapper<T> tmp = *this;
      ++*this;
      return tmp;
    }

#if __cplusplus >= 201103L
    template<typename U>
      void operator,(const U&) const = delete;
#else
  private:
    template<typename U>
      void operator,(const U&) const;
#endif
  };

#if __cplusplus >= 201103L
  template<typename T, typename U>
    void operator,(const T&, const output_iterator_wrapper<U>&) = delete;
#endif

#if __cplusplus >= 201103L
  using std::remove_cv;
#else
  template<typename T> struct remove_cv { typedef T type; };
  template<typename T> struct remove_cv<const T> { typedef T type; };
  template<typename T> struct remove_cv<volatile T> { typedef T type; };
  template<typename T> struct remove_cv<const volatile T> { typedef T type; };
#endif

  /**
   * @brief input_iterator wrapper for pointer
   *
   * This class takes a pointer and wraps it to provide exactly
   * the requirements of a input_iterator. It should not be
   * instantiated directly, but generated from a test_container
   */
  template<class T>
  class input_iterator_wrapper
  : public std::iterator<std::input_iterator_tag, typename remove_cv<T>::type,
			 std::ptrdiff_t, T*, T&>
  {
  protected:
    input_iterator_wrapper() : ptr(0), SharedInfo(0)
    { }

  public:
    typedef BoundsContainer<T> ContainerType;
    T* ptr;
    ContainerType* SharedInfo;

    input_iterator_wrapper(T* _ptr, ContainerType* SharedInfo_in)
    : ptr(_ptr), SharedInfo(SharedInfo_in)
    { ITERATOR_VERIFY(ptr >= SharedInfo->first && ptr <= SharedInfo->last); }

#if __cplusplus >= 201103L
    input_iterator_wrapper(const input_iterator_wrapper&) = default;

    input_iterator_wrapper&
    operator=(const input_iterator_wrapper&) = default;
#endif

    bool
    operator==(const input_iterator_wrapper& in) const
    {
      ITERATOR_VERIFY(SharedInfo && SharedInfo == in.SharedInfo);
      ITERATOR_VERIFY(ptr>=SharedInfo->first && in.ptr>=SharedInfo->first);
      return ptr == in.ptr;
    }

    bool
    operator!=(const input_iterator_wrapper& in) const
    {
      return !(*this == in);
    }

    T&
    operator*() const
    {
      ITERATOR_VERIFY(SharedInfo && ptr < SharedInfo->last);
      ITERATOR_VERIFY(ptr >= SharedInfo->first);
      return *ptr;
    }

    T*
    operator->() const
    {
      return &**this;
    }

    input_iterator_wrapper&
    operator++()
    {
      ITERATOR_VERIFY(SharedInfo && ptr < SharedInfo->last);
      ITERATOR_VERIFY(ptr>=SharedInfo->first);
      ptr++;
      SharedInfo->first=ptr;
      return *this;
    }

    void
    operator++(int)
    {
      ++*this;
    }

#if __cplusplus >= 201103L
    template<typename U>
      void operator,(const U&) const = delete;
#else
  private:
    template<typename U>
      void operator,(const U&) const;
#endif
  };

#if __cplusplus >= 201103L
  template<typename T, typename U>
    void operator,(const T&, const input_iterator_wrapper<U>&) = delete;
#endif

  /**
   * @brief forward_iterator wrapper for pointer
   *
   * This class takes a pointer and wraps it to provide exactly
   * the requirements of a forward_iterator. It should not be
   * instantiated directly, but generated from a test_container
   */
  template<class T>
  struct forward_iterator_wrapper : public input_iterator_wrapper<T>
  {
    typedef BoundsContainer<T> ContainerType;
    typedef std::forward_iterator_tag iterator_category;

    forward_iterator_wrapper(T* _ptr, ContainerType* SharedInfo_in)
    : input_iterator_wrapper<T>(_ptr, SharedInfo_in)
    { }

    forward_iterator_wrapper()
    { }

#if __cplusplus >= 201103L
    forward_iterator_wrapper(const forward_iterator_wrapper&) = default;

    forward_iterator_wrapper&
    operator=(const forward_iterator_wrapper&) = default;
#endif

    T&
    operator*() const
    {
      ITERATOR_VERIFY(this->SharedInfo && this->ptr < this->SharedInfo->last);
      return *(this->ptr);
    }

    T*
    operator->() const
    { return &**this; }

    forward_iterator_wrapper&
    operator++()
    {
      ITERATOR_VERIFY(this->SharedInfo && this->ptr < this->SharedInfo->last);
      this->ptr++;
      return *this;
    }

    forward_iterator_wrapper
    operator++(int)
    {
      forward_iterator_wrapper<T> tmp = *this;
      ++*this;
      return tmp;
    }
  };

  /**
   * @brief bidirectional_iterator wrapper for pointer
   *
   * This class takes a pointer and wraps it to provide exactly
   * the requirements of a bidirectional_iterator. It should not be
   * instantiated directly, but generated from a test_container
   */
  template<class T>
  struct bidirectional_iterator_wrapper : public forward_iterator_wrapper<T>
  {
    typedef BoundsContainer<T> ContainerType;
    typedef std::bidirectional_iterator_tag iterator_category;

    bidirectional_iterator_wrapper(T* _ptr, ContainerType* SharedInfo_in)
    : forward_iterator_wrapper<T>(_ptr, SharedInfo_in)
    { }

    bidirectional_iterator_wrapper()
    : forward_iterator_wrapper<T>()
    { }

#if __cplusplus >= 201103L
    bidirectional_iterator_wrapper(
	const bidirectional_iterator_wrapper&) = default;

    bidirectional_iterator_wrapper&
    operator=(const bidirectional_iterator_wrapper&) = default;
#endif

    bidirectional_iterator_wrapper&
    operator++()
    {
      ITERATOR_VERIFY(this->SharedInfo && this->ptr < this->SharedInfo->last);
      this->ptr++;
      return *this;
    }

    bidirectional_iterator_wrapper
    operator++(int)
    {
      bidirectional_iterator_wrapper<T> tmp = *this;
      ++*this;
      return tmp;
    }

    bidirectional_iterator_wrapper&
    operator--()
    {
      ITERATOR_VERIFY(this->SharedInfo && this->ptr > this->SharedInfo->first);
      this->ptr--;
      return *this;
    }

    bidirectional_iterator_wrapper
    operator--(int)
    {
      bidirectional_iterator_wrapper<T> tmp = *this;
      --*this;
      return tmp;
    }
  };

  /**
   * @brief random_access_iterator wrapper for pointer
   *
   * This class takes a pointer and wraps it to provide exactly
   * the requirements of a random_access_iterator. It should not be
   * instantiated directly, but generated from a test_container
   */
  template<class T>
  struct random_access_iterator_wrapper
  : public bidirectional_iterator_wrapper<T>
  {
    typedef BoundsContainer<T> ContainerType;
    typedef std::random_access_iterator_tag iterator_category;

    random_access_iterator_wrapper(T* _ptr, ContainerType* SharedInfo_in)
    : bidirectional_iterator_wrapper<T>(_ptr, SharedInfo_in)
    { }

    random_access_iterator_wrapper()
    : bidirectional_iterator_wrapper<T>()
    { }

#if __cplusplus >= 201103L
    random_access_iterator_wrapper(
	const random_access_iterator_wrapper&) = default;

    random_access_iterator_wrapper&
    operator=(const random_access_iterator_wrapper&) = default;
#endif

    random_access_iterator_wrapper&
    operator++()
    {
      ITERATOR_VERIFY(this->SharedInfo && this->ptr < this->SharedInfo->last);
      this->ptr++;
      return *this;
    }

    random_access_iterator_wrapper
    operator++(int)
    {
      random_access_iterator_wrapper<T> tmp = *this;
      ++*this;
      return tmp;
    }

    random_access_iterator_wrapper&
    operator--()
    {
      ITERATOR_VERIFY(this->SharedInfo && this->ptr > this->SharedInfo->first);
      this->ptr--;
      return *this;
    }

    random_access_iterator_wrapper
    operator--(int)
    {
      random_access_iterator_wrapper<T> tmp = *this;
      --*this;
      return tmp;
    }

    random_access_iterator_wrapper&
    operator+=(std::ptrdiff_t n)
    {
      if(n > 0)
	{
	  ITERATOR_VERIFY(n <= this->SharedInfo->last - this->ptr);
	  this->ptr += n;
	}
      else
	{
	  ITERATOR_VERIFY(n <= this->ptr - this->SharedInfo->first);
	  this->ptr += n;
	}
      return *this;
    }

    random_access_iterator_wrapper&
    operator-=(std::ptrdiff_t n)
    { return *this += -n; }

    random_access_iterator_wrapper
    operator-(std::ptrdiff_t n) const
    {
      random_access_iterator_wrapper<T> tmp = *this;
      return tmp -= n;
    }

    std::ptrdiff_t
    operator-(const random_access_iterator_wrapper<T>& in) const
    {
      ITERATOR_VERIFY(this->SharedInfo == in.SharedInfo);
      return this->ptr - in.ptr;
    }

    T&
    operator[](std::ptrdiff_t n) const
    { return *(*this + n); }

    bool
    operator<(const random_access_iterator_wrapper<T>& in) const
    {
      ITERATOR_VERIFY(this->SharedInfo == in.SharedInfo);
      return this->ptr < in.ptr;
    }

    bool
    operator>(const random_access_iterator_wrapper<T>& in) const
    {
      return in < *this;
    }

    bool
    operator>=(const random_access_iterator_wrapper<T>& in) const
    {
      return !(*this < in);
    }

    bool
    operator<=(const random_access_iterator_wrapper<T>& in) const
    {
      return !(*this > in);
    }
  };

  template<typename T>
    random_access_iterator_wrapper<T>
    operator+(random_access_iterator_wrapper<T> it, std::ptrdiff_t n)
    { return it += n; }

  template<typename T>
    random_access_iterator_wrapper<T>
    operator+(std::ptrdiff_t n, random_access_iterator_wrapper<T> it)
    { return it += n; }


  /**
   * @brief A container-type class for holding iterator wrappers
   * test_container takes two parameters, a class T and an iterator
   * wrapper templated by T (for example forward_iterator_wrapper<T>.
   * It takes two pointers representing a range and presents them as
   * a container of iterators.
   */
  template <class T, template<class TT> class ItType>
  struct test_container
  {
    typename ItType<T>::ContainerType bounds;

    test_container(T* _first, T* _last) : bounds(_first, _last)
    { }

#if __cplusplus >= 201103L
    template<std::size_t N>
      explicit
      test_container(T (&arr)[N]) : test_container(arr, arr+N)
      { }
#endif

    ItType<T>
    it(int pos)
    {
      ITERATOR_VERIFY(pos >= 0 && pos <= size());
      return ItType<T>(bounds.first + pos, &bounds);
    }

    ItType<T>
    it(T* pos)
    {
      ITERATOR_VERIFY(pos >= bounds.first && pos <= bounds.last);
      return ItType<T>(pos, &bounds);
    }

    const T&
    val(int pos)
    { return (bounds.first)[pos]; }

    ItType<T>
    begin()
    { return it(bounds.first); }

    ItType<T>
    end()
    { return it(bounds.last); }

    std::size_t
    size() const
    { return bounds.size(); }
  };

#if __cplusplus > 201703L
  template<typename T>
    struct contiguous_iterator_wrapper
    : random_access_iterator_wrapper<T>
    {
      using random_access_iterator_wrapper<T>::random_access_iterator_wrapper;

      using iterator_concept = std::contiguous_iterator_tag;

      contiguous_iterator_wrapper&
      operator++()
      {
	random_access_iterator_wrapper<T>::operator++();
	return *this;
      }

      contiguous_iterator_wrapper&
      operator--()
      {
	random_access_iterator_wrapper<T>::operator--();
	return *this;
      }

      contiguous_iterator_wrapper
      operator++(int)
      {
	auto tmp = *this;
	++*this;
	return tmp;
      }

      contiguous_iterator_wrapper
      operator--(int)
      {
	auto tmp = *this;
	--*this;
	return tmp;
      }

      contiguous_iterator_wrapper&
      operator+=(std::ptrdiff_t n)
      {
	random_access_iterator_wrapper<T>::operator+=(n);
	return *this;
      }

      friend contiguous_iterator_wrapper
      operator+(contiguous_iterator_wrapper iter, std::ptrdiff_t n)
      { return iter += n; }

      friend contiguous_iterator_wrapper
      operator+(std::ptrdiff_t n, contiguous_iterator_wrapper iter)
      { return iter += n; }

      contiguous_iterator_wrapper&
      operator-=(std::ptrdiff_t n)
      { return *this += -n; }

      friend contiguous_iterator_wrapper
      operator-(contiguous_iterator_wrapper iter, std::ptrdiff_t n)
      { return iter -= n; }
    };

  // A type meeting the minimum std::range requirements
  template<typename T, template<typename> class Iter>
    class test_range
    {
      // Adds default constructor to Iter<T> if needed
      struct iterator : Iter<T>
      {
	using Iter<T>::Iter;

	iterator() : Iter<T>(nullptr, nullptr) { }

	using Iter<T>::operator++;

	iterator& operator++() { Iter<T>::operator++(); return *this; }
      };

      template<typename I>
	struct sentinel
	{
	  T* end;

	  friend bool operator==(const sentinel& s, const I& i)
	  { return s.end == i.ptr; }
	};

      auto
      get_iterator(T* p)
      {
	if constexpr (std::default_initializable<Iter<T>>)
	  return Iter<T>(p, &bounds);
	else
	  return iterator(p, &bounds);
      }

    public:
      test_range(T* first, T* last) : bounds(first, last)
      { }

      template<std::size_t N>
	explicit
	test_range(T (&arr)[N]) : test_range(arr, arr+N)
	{ }

      auto begin() & { return get_iterator(bounds.first); }

      auto end() &
      {
	using I = decltype(get_iterator(bounds.last));
	if constexpr (std::sentinel_for<I, I>)
	  return get_iterator(bounds.last);
	else
	  return sentinel<I>{bounds.last};
      }

      typename Iter<T>::ContainerType bounds;
    };

  template<typename T>
    using test_contiguous_range
      = test_range<T, contiguous_iterator_wrapper>;
  template<typename T>
    using test_random_access_range
      = test_range<T, random_access_iterator_wrapper>;
  template<typename T>
    using test_bidirectional_range
      = test_range<T, bidirectional_iterator_wrapper>;
  template<typename T>
    using test_forward_range
      = test_range<T, forward_iterator_wrapper>;
  template<typename T>
    using test_input_range
      = test_range<T, input_iterator_wrapper>;
  template<typename T>
    using test_output_range
      = test_range<T, output_iterator_wrapper>;

  // A type meeting the minimum std::sized_range requirements
  template<typename T, template<typename> class Iter>
    struct test_sized_range : test_range<T, Iter>
    {
      using test_range<T, Iter>::test_range;

      std::size_t size() const noexcept
      { return this->bounds.size(); }
    };

  template<typename T>
    using test_contiguous_sized_range
      = test_sized_range<T, contiguous_iterator_wrapper>;
  template<typename T>
    using test_random_access_sized_range
      = test_sized_range<T, random_access_iterator_wrapper>;
  template<typename T>
    using test_bidirectional_sized_range
      = test_sized_range<T, bidirectional_iterator_wrapper>;
  template<typename T>
    using test_forward_sized_range
      = test_sized_range<T, forward_iterator_wrapper>;
  template<typename T>
    using test_input_sized_range
      = test_sized_range<T, input_iterator_wrapper>;
  template<typename T>
    using test_output_sized_range
      = test_sized_range<T, output_iterator_wrapper>;
#endif // C++20
} // namespace __gnu_test
#endif // _TESTSUITE_ITERATORS
