#![feature(no_core, lang_items)]
#![no_core]
#[lang="sized"] trait Sized {}
struct Foo<const N: usize> { data: [u8; N] }
fn main() { let _: Foo<_> = Foo { data: [1,2,3] }; }
