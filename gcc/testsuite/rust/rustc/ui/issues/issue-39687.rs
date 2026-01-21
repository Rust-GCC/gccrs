#![feature(fn_traits)]

fn main() {
    <fn() as Fn()>::call;
// { dg-error ".E0229." "" { target *-*-* } .-1 }
}

