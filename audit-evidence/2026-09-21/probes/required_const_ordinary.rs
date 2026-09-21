#![feature(no_core, lang_items, rustc_attrs)]
#![no_core]
#[lang="sized"] trait Sized {}
#[rustc_args_required_const(1)] fn foo(x:i32,n:i32)->i32{x+n}
fn main()->i32 { foo(10,3)-13 }
