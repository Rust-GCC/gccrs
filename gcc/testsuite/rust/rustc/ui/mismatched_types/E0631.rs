#![feature(unboxed_closures)]

fn foo<F: Fn(usize)>(_: F) {}
fn bar<F: Fn<usize>>(_: F) {}
fn main() {
    fn f(_: u64) {}
    foo(|_: isize| {}); // { dg-error ".E0631." "" { target *-*-* } }
    bar(|_: isize| {}); // { dg-error ".E0631." "" { target *-*-* } }
    foo(f); // { dg-error ".E0631." "" { target *-*-* } }
    bar(f); // { dg-error ".E0631." "" { target *-*-* } }
}

