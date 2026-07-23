// Regression test for #69615.

#![feature(const_trait_impl, const_fn)]
#![allow(incomplete_features)]

pub trait MyTrait {
    fn method(&self) -> Option<()>;
}

impl const MyTrait for () {
    fn method(&self) -> Option<()> {
        Some(())?; // { dg-error ".E0744." "" { target *-*-* } }
        None
    }
}

fn main() {}

