#![feature(no_core, lang_items)]
#![no_core]

#[lang = "sized"]
trait Sized {}

#[lang = "deref"]
pub trait Deref {
    type Target: ?Sized;

    fn deref(&self) -> &Self::Target;
}

pub trait Hash {
    fn hash<H: Hasher>(&self, state: &mut H);

    // FWIW, this is the function that creates the invalid object-safe error on `Hash for Pin<P>`
    fn hash_slice<H: Hasher>(data: &[Self], state: &mut H)
    where
        Self: Sized,
    {}
}

#[stable(feature = "rust1", since = "1.0.0")]
pub trait Hasher {}

#[stable(feature = "pin", since = "1.33.0")]
#[lang = "pin"]
#[fundamental]
#[repr(transparent)]
pub struct Pin<P> {
    _pointer: P,
}

impl<P: Deref<Target: Hash>> Hash for Pin<P> { // { dg-warning "not object safe" }
    fn hash<H: Hasher>(&self, state: &mut H) {
        P::Target::hash(self, state);
    }
}
