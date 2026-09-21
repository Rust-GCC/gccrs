#![feature(no_core, lang_items, rustc_attrs)]
#![no_core]
#[lang="sized"] trait Sized {}
#[rustc_legacy_const_generics(1)] fn foo<const N: usize>(x: i32)->i32{x+N as i32}
fn main()->i32 { foo(10,3)-13 }
