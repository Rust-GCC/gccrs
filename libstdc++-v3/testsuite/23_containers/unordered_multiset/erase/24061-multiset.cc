// { dg-do run { target c++11 } }

// 2010-02-10  Paolo Carlini  <paolo.carlini@oracle.com> 
//
// Copyright (C) 2010-2023 Free Software Foundation, Inc.
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

#include <unordered_set>
#include <string>
#include <testsuite_hooks.h>

namespace
{
  std::size_t
  get_nb_bucket_elems(const std::unordered_multiset<std::string>& us)
  {
    std::size_t nb = 0;
    for (std::size_t b = 0; b != us.bucket_count(); ++b)
      {
	nb += us.bucket_size(b);
      }
    return nb;
  }
}

// libstdc++/24061
void test01()
{
  typedef std::unordered_multiset<std::string> Mset;
  typedef Mset::iterator       iterator;
  typedef Mset::const_iterator const_iterator;

  Mset ms1;
  
  ms1.insert("all the love in the world");
  ms1.insert("you know what you are?");
  ms1.insert("the collector");
  ms1.insert("the hand that feeds");
  ms1.insert("love is not enough");
  ms1.insert("every day is exactly the same");
  ms1.insert("with teeth");
  ms1.insert("only");
  ms1.insert("getting smaller");
  ms1.insert("sunspots");

  ms1.insert("the hand that feeds");
  ms1.insert("love is not enough");
  ms1.insert("every day is exactly the same");
  VERIFY( ms1.size() == 13 );
  VERIFY( get_nb_bucket_elems(ms1) == ms1.size() );

  iterator it1 = ms1.begin();
  ++it1;
  iterator it2 = it1;
  ++it2;
  iterator it3 = ms1.erase(it1);
  VERIFY( ms1.size() == 12 );
  VERIFY( get_nb_bucket_elems(ms1) == ms1.size() );
  VERIFY( it3 == it2 );
  VERIFY( *it3 == *it2 );

  iterator it4 = ms1.begin();
  ++it4;
  ++it4;
  ++it4;
  iterator it5 = it4;
  ++it5;
  ++it5;
  iterator it6 = ms1.erase(it4, it5);
  VERIFY( ms1.size() == 10 );
  VERIFY( get_nb_bucket_elems(ms1) == ms1.size() );
  VERIFY( it6 == it5 );
  VERIFY( *it6 == *it5 );

  const_iterator it7 = ms1.begin();
  ++it7;
  ++it7;
  ++it7;
  const_iterator it8 = it7;
  ++it8;
  const_iterator it9 = ms1.erase(it7);
  VERIFY( ms1.size() == 9 );
  VERIFY( get_nb_bucket_elems(ms1) == ms1.size() );
  VERIFY( it9 == it8 );
  VERIFY( *it9 == *it8 );

  const_iterator it10 = ms1.begin();
  ++it10;
  const_iterator it11 = it10;
  ++it11;
  ++it11;
  ++it11;
  ++it11;
  const_iterator it12 = ms1.erase(it10, it11);
  VERIFY( ms1.size() == 5 );
  VERIFY( get_nb_bucket_elems(ms1) == ms1.size() );
  VERIFY( it12 == it11 );
  VERIFY( *it12 == *it11 );

  iterator it13 = ms1.erase(ms1.begin(), ms1.end());
  VERIFY( ms1.size() == 0 );
  VERIFY( get_nb_bucket_elems(ms1) == ms1.size() );
  VERIFY( it13 == ms1.end() );
  VERIFY( it13 == ms1.begin() );
}
  
int main()
{
  test01();
  return 0;
}
