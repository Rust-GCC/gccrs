! Test f_c_string cases that can be fully constant-folded
! { dg-do compile }
! { dg-additional-options "-fdump-tree-original" }

use iso_c_binding, only: f_c_string, c_char
implicit none (external, type)
character(*, c_char), parameter :: str1 = "blah1"
character(*, c_char), parameter :: str2 = "blah2"
character(*, c_char), parameter :: str3 = "blah3"
character(*, c_char), parameter :: str4 = "blah4   "
character(*, c_char), parameter :: str5 = "blah5   "
character(*, c_char), parameter :: str6 = "blah6   "
external foo

call foo(f_c_string("hello world1", asis=.true.))
! { dg-final { scan-tree-dump-times "hello world1.\[^\\n\\r\]*, 13" 1 "original" } }

call foo(f_c_string("hello world2", asis=.false.))
! { dg-final { scan-tree-dump-times "hello world2.\[^\\n\\r\]*, 13" 1 "original" } }

call foo(f_c_string("hello world3"))
! { dg-final { scan-tree-dump-times "hello world3.\[^\\n\\r\]*, 13" 1 "original" } }

call foo(f_c_string("hello1 ", asis=.true.))
! { dg-final { scan-tree-dump-times "hello1 .\[^\\n\\r\]*, 8" 1 "original" } }

call foo(f_c_string("hello2 ", asis=.false.))
! { dg-final { scan-tree-dump-times "hello2.\[^\\n\\r\]*, 7" 1 "original" } }

call foo(f_c_string("hello3 "))
! { dg-final { scan-tree-dump-times "hello3.\[^\\n\\r\]*, 7" 1 "original" } }

call foo(f_c_string(str1, asis=.true.))
! { dg-final { scan-tree-dump-times "blah1.\[^\\n\\r\]*, 6" 1 "original" } }

call foo(f_c_string(str2, asis=.false.))
! { dg-final { scan-tree-dump-times "blah2.\[^\\n\\r\]*, 6" 1 "original" } }

call foo(f_c_string(str3))
! { dg-final { scan-tree-dump-times "blah3.\[^\\n\\r\]*, 6" 1 "original" } }

call foo(f_c_string(str4, asis=.true.))
! { dg-final { scan-tree-dump-times "blah4   .\[^\\n\\r\]*, 9" 1 "original" } }

call foo(f_c_string(str5, asis=.false.))
! { dg-final { scan-tree-dump-times "blah5.\[^\\n\\r\]*, 6" 1 "original" } }

call foo(f_c_string(str6))
! { dg-final { scan-tree-dump-times "blah6.\[^\\n\\r\]*, 6" 1 "original" } }

end


