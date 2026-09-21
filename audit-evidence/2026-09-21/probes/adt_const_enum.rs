#![feature(no_core, lang_items)]
#![no_core]
#[lang="sized"] trait Sized {}
enum Key { A, B }
struct Foo<const K: Key>;
impl Foo<{Key::A}> { fn val(&self)->i32 { 1 } }
impl Foo<{Key::B}> { fn val(&self)->i32 { 2 } }
fn main()->i32 { let x=Foo::<{Key::A}> {}; x.val()-1 }
