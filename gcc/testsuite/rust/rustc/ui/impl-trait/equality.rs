#![feature(specialization)] // { dg-warning "" "" { target *-*-* } }

trait Foo: Copy + ToString {}

impl<T: Copy + ToString> Foo for T {}

fn hide<T: Foo>(x: T) -> impl Foo {
    x
}

fn two(x: bool) -> impl Foo {
    if x {
        return 1_i32;
    }
    0_u32
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

fn sum_to(n: u32) -> impl Foo {
    if n == 0 {
        0
    } else {
        n + sum_to(n - 1)
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    }
}

trait Leak: Sized {
    type T;
    fn leak(self) -> Self::T;
}
impl<T> Leak for T {
    default type T = ();
    default fn leak(self) -> Self::T { panic!() }
}
impl Leak for i32 {
    type T = i32;
    fn leak(self) -> i32 { self }
}

fn main() {
}

