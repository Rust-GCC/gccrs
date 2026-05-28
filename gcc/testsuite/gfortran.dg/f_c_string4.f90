! Test f_c_string cases with constant strings but that need a conditional.
! { dg-do compile }
! { dg-additional-options "-fdump-tree-original" }

subroutine doit (x)

use iso_c_binding, only: f_c_string, c_char
implicit none (external, type)
logical :: x
character(*, c_char), parameter :: str1 = "blah1"
character(*, c_char), parameter :: str2 = "blah2   "
external foo

call foo(f_c_string("hello world1", asis=x))
! { dg-final { scan-tree-dump-times "hello world1.\[^\\n\\r\]*, 13" 1 "original" } }

call foo(f_c_string("hello1 ", asis=x))
! { dg-final { scan-tree-dump-times "hello1 .\[^\\n\\r\]* 8 : 7" 1 "original" } }

call foo(f_c_string(str1, asis=x))
! { dg-final { scan-tree-dump-times "blah1.\[^\\n\\r\]*, 6" 1 "original" } }

call foo(f_c_string(str2, asis=x))
! { dg-final { scan-tree-dump-times "blah2   .\[^\\n\\r\]* 9 : 6" 1 "original" } }

end subroutine
