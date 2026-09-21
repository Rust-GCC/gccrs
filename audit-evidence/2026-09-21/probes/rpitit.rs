#![feature(no_core, lang_items)]
#![no_core]
#[lang="sized"] trait Sized {}
trait Foo { fn a(&self) -> impl Foo; }
struct S; impl Foo for S { fn a(&self) -> impl Foo { S } }
fn main() { let _ = S.a(); }
