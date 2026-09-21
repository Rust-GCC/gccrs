#![feature(no_core, lang_items)]
#![no_core]
#[lang="sized"] trait Sized {}
struct Foo<const N: usize> { data: [u8; N + 1] }
fn main() { let _: Foo<2> = Foo { data: [1,2,3] }; }
