// { dg-additional-options "-w" }
#![feature(lang_items)]
#[lang = "sized"]
pub trait Sized {}

#[lang = "const_ptr"]
impl<T> *const T {
    fn test(self) {}
}
