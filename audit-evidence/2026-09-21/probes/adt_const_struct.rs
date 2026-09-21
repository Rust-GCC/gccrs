#![feature(no_core, lang_items)]
#![no_core]
#[lang="sized"] trait Sized {}
struct Key { n: usize }
struct Foo<const K: Key>;
impl<const K: Key> Foo<K> { fn val(&self) -> usize { K.n } }
fn main() -> i32 { let x = Foo::<{ Key { n: 7 } }> {}; x.val() as i32 - 7 }
