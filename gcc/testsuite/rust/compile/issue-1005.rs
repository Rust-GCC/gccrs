// { dg-additional-options "-w" }
#[lang = "sized"]
pub trait Sized {}

#[lang = "const_ptr"]
impl<T> *const T {
    fn test(self) {}
}
