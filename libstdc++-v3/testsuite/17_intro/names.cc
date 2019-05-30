// Copyright (C) 2017-2019 Free Software Foundation, Inc.
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

// { dg-do compile }

// Define macros for some common variables names that we must not use for
// naming variables, parameters etc. in the library.
#define A (
#define B (
#define C (
#define D (
#define E (
#define F (
#define G (
#define H (
#define I (
#define J (
#define K (
#define L (
#define M (
#define N (
#define O (
#define P (
#define Q (
#define R (
#define S (
#define T (
#define U (
#define V (
#define W (
#define X (
#define Y (
#define Z (
#if __cplusplus >= 201103L
// <random> defines member functions called a() and b()
#else
#define a (
#define b (
#endif
// <queue> and <stack> defined data members called c
#define d (
#define e (
#define f (
#define g (
#if __cplusplus >= 201402L
// <chrono> defines operator ""h in C++14
// <complex> defines operator ""i in C++14
#else
#define h (
#define i (
#endif
#define j (
#if __cplusplus >= 201103L
// <random> defines member functions called k()
#else
#define k (
#endif
#define l (
#if __cplusplus >= 201103L
// <random> defines member functions called m() and n()
#else
#define m (
#define n (
#endif
#define o (
#if __cplusplus >= 201103L
// <random> defines member functions called p()
#else
#define p (
#endif
#define q (
#define r (
#if __cplusplus >= 201103L
// <random> defines member functions called s() and t()
// <chrono> and <string> define operator ""s in C++14
#else
#define s (
#define t (
#endif
#define u (
#define v (
#define w (
#define x (
#define y (
#define z (

#define tmp (

#if __cplusplus < 201703L
// <charconv> defines to_chars_result::ptr and to_chars_result::ec
#define ec (
#define ptr (
#endif

// Common template parameter names
#define OutputIterator		OutputIterator is not a reserved name
#define InputIterator		InputIterator is not a reserved name
#define ForwardIterator		ForwardIterator is not a reserved name
#define BidirectionalIterator	BidirectionalIterator is not a reserved name
#define RandomAccessIterator	RandomAccessIterator is not a reserved name
#define RandomAccessOutputIterator	RandomAccessOutputIterator is not a reserved name
#define RAIter			RAIter is not a reserved name
#define FwdIter			FwdIter is not a reserved name
#define OutIter			OutIter is not a reserved name
#define InIter			InIter is not a reserved name

#define Alloc			Alloc is not a reserved name
#define BinaryFunction1		BinaryFunction1 is not a reserved name
#define BinaryFunction2		BinaryFunction2 is not a reserved name
#define Char			Char is not a reserved name
#define CharT			CharT is not a reserved name
#define Cmp			Cmp is not a reserved name
#define Compare			Compare is not a reserved name
#define Const_Iterator		Const_Iterator is not a reserved name
#define Const_Key_Reference	Const_Key_Reference is not a reserved name
#define Const_Node_Iter		Const_Node_Iter is not a reserved name
#define Const_Pointer		Const_Pointer is not a reserved name
#define Const_Reference		Const_Reference is not a reserved name
#define Data			Data is not a reserved name
#define DiffType		DiffType is not a reserved name
#define Eq			Eq is not a reserved name
#define ExecPolicy		ExecPolicy is not a reserved name
#define Expected		Expected is not a reserved name
#define External_Load_Access	External_Load_Access is not a reserved name
#define External_Size_Access	External_Size_Access is not a reserved name
#define Fn			Fn is not a reserved name
#define Function		Function is not a reserved name
#define Functor			Functor is not a reserved name
#define Hash			Hash is not a reserved name
#define H1			H1 is not a reserved name
#define H2			H2 is not a reserved name
#define Head			Head is not a reserved name
#define It			It is not a reserved name
#define Iter			Iter is not a reserved name
#define Iterator		Iterator is not a reserved name
#define Key			Key is not a reserved name
#define Mapped			Mapped is not a reserved name
#define Node			Node is not a reserved name
#define Node_iter		Node_iter is not a reserved name
#define Node_ptr		Node_ptr is not a reserved name
#define Overflow		Overflow is not a reserved name
#define Pointer			Pointer is not a reserved name
#define Policy			Policy is not a reserved name
#define Pred			Pred is not a reserved name
#define Ptr			Ptr is not a reserved name
#define Reference		Reference is not a reserved name
#define Seq			Seq is not a reserved name
#define Seq_RAIter		Seq_RAIter is not a reserved name
#define Series			Series is not a reserved name
#define Set			Set is not a reserved name
#define String			String is not a reserved name
#define Tp			Tp is not a reserved name
#define Traits			Traits is not a reserved name
#define Type			Type is not a reserved name
#define Value			Value is not a reserved name
#define ValueT			ValueT is not a reserved name
#define ValueType		ValueType is not a reserved name

#ifdef _AIX
// See https://gcc.gnu.org/ml/libstdc++/2017-03/msg00015.html
#undef f
#undef r
#undef x
#undef y
#endif

#ifdef __hpux__
#undef d
#undef r
#endif

#if defined (__linux__) && defined (__sparc__)
#undef y
#endif

#ifdef __sun__
// See https://gcc.gnu.org/ml/libstdc++/2019-05/msg00175.html
#undef ptr
#endif

#include <bits/stdc++.h>
