#![feature(nll)]

// Test that we enforce user-provided type annotations on closures.

fn foo<'a>() {
    // Here `x` is free in the closure sig:
    |x: &'a i32| -> &'static i32 {
        return x; // { dg-error "" "" { target *-*-* } }
    };
}

fn foo1() {
    // Here `x` is bound in the closure sig:
    |x: &i32| -> &'static i32 {
        return x; // { dg-error "" "" { target *-*-* } }
    };
}

fn bar<'a>() {
    // Here `x` is free in the closure sig:
    |x: &'a i32, b: fn(&'static i32)| {
        b(x); // { dg-error "" "" { target *-*-* } }
    };
}

fn bar1() {
    // Here `x` is bound in the closure sig:
    |x: &i32, b: fn(&'static i32)| {
        b(x); // { dg-error ".E0521." "" { target *-*-* } }
    };
}

fn main() {}

