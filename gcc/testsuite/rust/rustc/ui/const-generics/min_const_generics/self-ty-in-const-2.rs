#![feature(min_const_generics)]

struct Bar<T>(T);

trait Baz {
    fn hey();
}

impl Baz for u16 {
    fn hey() {
        let _: [u8; std::mem::size_of::<Self>()]; // ok
    }
}

impl<T> Baz for Bar<T> {
    fn hey() {
        let _: [u8; std::mem::size_of::<Self>()]; // { dg-error "" "" { target *-*-* } }
    }
}

fn main() {}

