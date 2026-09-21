#![feature(no_core, lang_items)]
#![no_core]
#[lang="sized"] trait Sized {}
enum Key { A, B }
struct Foo<const K: Key>;
fn main() { let _: Foo<{Key::A}> = Foo::<{Key::B}> {}; }
