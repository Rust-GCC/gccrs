// Regression test for #68656

#![feature(generic_associated_types)]
// { dg-warning "" "" { target *-*-* } .-1 }

trait UnsafeCopy<T: Copy> {
    type Item<'a>: std::ops::Deref<Target = T>;

    fn bug<'a>(item: &Self::Item<'a>) -> () {
        let x: T = **item;
        &x as *const _;
    }
}

impl<T: Copy + std::ops::Deref> UnsafeCopy<T> for T {
    type Item<'a> = T;
// { dg-error ".E0271." "" { target *-*-* } .-1 }
}

fn main() {
    <&'static str>::bug(&"");
}

