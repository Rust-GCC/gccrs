#![feature(no_core, lang_items)]
#![no_core]
#[lang="sized"] trait Sized {}
trait ForeignOwnable: Sized {
    type Borrowed<'a> where Self: 'a;
    fn borrow<'a>(&'a self) -> Self::Borrowed<'a>;
}
struct S(i32);
impl ForeignOwnable for S {
    type Borrowed<'a> = &'a i32 where Self: 'a;
    fn borrow<'a>(&'a self) -> Self::Borrowed<'a> { &self.0 }
}
fn main() { let s=S(1); let _=s.borrow(); }
