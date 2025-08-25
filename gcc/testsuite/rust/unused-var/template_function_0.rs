#[lang = "sized"]
pub trait Sized {}

pub fn test<T> (a: usize) -> () {
    // { dg-warning "unused name" "" { target *-*-* } .-1 }
}