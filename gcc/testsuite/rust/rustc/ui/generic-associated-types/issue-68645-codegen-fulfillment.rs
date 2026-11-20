// Regression test for #68645

#![feature(generic_associated_types)]
// { dg-warning "" "" { target *-*-* } .-1 }

trait Fun {
    type F<'a>: Fn() -> u32;

    fn callme<'a>(f: Self::F<'a>) -> u32 {
        f()
    }
}

impl<T> Fun for T {
    type F<'a> = Self;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {
    <&dyn Iterator<Item = u8>>::callme(&std::iter::once(1));
}

