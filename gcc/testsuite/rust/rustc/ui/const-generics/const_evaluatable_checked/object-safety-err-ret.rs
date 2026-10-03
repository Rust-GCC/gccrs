#![feature(const_generics, const_evaluatable_checked)]
#![allow(incomplete_features)]


const fn bar<T: ?Sized>() -> usize { 7 }

trait Foo {
    fn test(&self) -> [u8; bar::<Self>()];
}

impl Foo for () {
    fn test(&self) -> [u8; bar::<Self>()] {
        [0; bar::<Self>()]
    }
}

fn use_dyn(v: &dyn Foo) { // { dg-error ".E0038." "" { target *-*-* } }
    v.test();
}

fn main() {}

