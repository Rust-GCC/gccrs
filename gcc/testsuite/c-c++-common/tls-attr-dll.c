/* { dg-do compile { target i?86-*-mingw* x86_64-*-mingw* } } */

__declspec (dllimport) __thread int foo1; /* { dg-error "thread-local variable" } */

__declspec (dllexport) __thread int foo2; /* { dg-error "thread-local variable" } */
