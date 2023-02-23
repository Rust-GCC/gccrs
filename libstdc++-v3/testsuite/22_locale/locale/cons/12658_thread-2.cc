// { dg-do run }
// { dg-options "-pthread"  }
// { dg-require-effective-target pthread }
// { dg-require-namedlocale "en_US.ISO8859-1" }
// { dg-require-namedlocale "fr_FR.ISO8859-15" }

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

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// 22.1.1.2 locale constructors and destructors [lib.locale.cons]

#include <locale>
#include <pthread.h>
#include <testsuite_hooks.h>

const int max_thread_count = 20;
//const int max_loop_count = 1000000; // orig value
const int max_loop_count = 100000;
const int max_locales = 10;
std::locale loc[max_locales];

void* thread_main(void*)
{
  try
    {
      for (int i = 0; i < max_loop_count; ++i)
	{
	  int k = i % max_locales;
	  std::locale::global(loc[k]);
	}
    }
  catch (...) { }
  return 0;
}
 
int
main()
{
  pthread_t tid[max_thread_count];
  
  for (int j = 0; j < max_locales; ++j)
    loc[j] = std::locale(j % 2 ? ISO_8859(1,en_US) : ISO_8859(15,fr_FR));

  for (int i = 0; i < max_thread_count; i++)
    pthread_create(&tid[i], 0, thread_main, 0);
  
  for (int i = 0; i < max_thread_count; i++)
    pthread_join(tid[i], 0);

  return 0;
}
