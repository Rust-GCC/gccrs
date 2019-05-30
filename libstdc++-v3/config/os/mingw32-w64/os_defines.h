// Specific definitions for generic platforms  -*- C++ -*-

// Copyright (C) 2000-2019 Free Software Foundation, Inc.
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

/** @file bits/os_defines.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iosfwd}
 */

#ifndef _GLIBCXX_OS_DEFINES
#  define _GLIBCXX_OS_DEFINES

// System-specific #define, typedefs, corrections, etc, go here.  This
// file will come before all others.

//  Define as 0, if you want, to enable inlining of gthread functions.
//  By default, don't pollute libstdc++ with win32api names.
#if !defined (__GTHREAD_HIDE_WIN32API)
# define __GTHREAD_HIDE_WIN32API 1
#endif

// Don't let win32api windef.h define min and max as macros
// if included after c++config.h.
#undef NOMINMAX
#define NOMINMAX 1

// Make sure that POSIX printf/scanf functions are activated.  As
// libstdc++ depends on POSIX-definitions of those functions, we define
// it unconditionally.
#undef __USE_MINGW_ANSI_STDIO
#define __USE_MINGW_ANSI_STDIO 1

#if defined (_GLIBCXX_DLL)
#define _GLIBCXX_PSEUDO_VISIBILITY_default __attribute__ ((__dllimport__))
#else
#define _GLIBCXX_PSEUDO_VISIBILITY_default
#endif
#define _GLIBCXX_PSEUDO_VISIBILITY_hidden

#define _GLIBCXX_PSEUDO_VISIBILITY(V) _GLIBCXX_PSEUDO_VISIBILITY_ ## V

// See libstdc++/20806.
#define _GLIBCXX_HAVE_DOS_BASED_FILESYSTEM 1

// See libstdc++/43738
// On native windows targets there is no ioctl function. And the existing
// ioctlsocket function doesn't work for normal file-descriptors.
#define _GLIBCXX_NO_IOCTL 1

// See libstdc++/51135
// Class constructors/destructors have __thiscall calling-convention on
// IA 32-bit
#if defined (__i386__)
#define _GLIBCXX_CDTOR_CALLABI __thiscall
#endif

#ifdef __x86_64__
#define _GLIBCXX_LLP64 1
// See libstdc++/69506
#define _GLIBCXX_USE_WEAK_REF 0
#endif

// Enable use of GetModuleHandleEx (requires Windows XP/2003) in
// __cxa_thread_atexit to prevent modules from being unloaded before
// their dtors are called
#define _GLIBCXX_THREAD_ATEXIT_WIN32 1

// See libstdc++/59807
#define _GTHREAD_USE_MUTEX_INIT_FUNC 1

#define _GLIBCXX_USE_CRT_RAND_S 1

#endif
