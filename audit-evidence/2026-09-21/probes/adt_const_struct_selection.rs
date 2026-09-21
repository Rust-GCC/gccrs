#![feature(no_core, lang_items)]
#![no_core]
#[lang="sized"] trait Sized {}
struct Key { n: usize }
struct Foo<const K: Key>;
impl Foo<{ Key {n:1} }> { fn val(&self)->i32 { 1 } }
impl Foo<{ Key {n:2} }> { fn val(&self)->i32 { 2 } }
fn main()->i32 { let x=Foo::<{Key{n:1}}> {}; let y=Foo::<{Key{n:2}}> {}; x.val()+y.val()-3 }
