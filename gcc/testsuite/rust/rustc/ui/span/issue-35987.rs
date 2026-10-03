struct Foo<T: Clone>(T);

use std::ops::Add;

impl<T: Clone, Add> Add for Foo<T> {
// { dg-error ".E0404." "" { target *-*-* } .-1 }
    type Output = usize;

    fn add(self, rhs: Self) -> Self::Output {
        unimplemented!();
    }
}

fn main() {}

