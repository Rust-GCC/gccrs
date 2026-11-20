// regression test for #74018

#![feature(type_alias_impl_trait)]

trait Trait {
    type Associated;
    fn into(self) -> Self::Associated;
}

impl<'a, I: Iterator<Item = i32>> Trait for (i32, I) {
// { dg-error ".E0207." "" { target *-*-* } .-1 }
    type Associated = (i32, impl Iterator<Item = i32>);
    fn into(self) -> Self::Associated {
        (0_i32, [0_i32].iter().copied())
    }
}

fn main() {}

