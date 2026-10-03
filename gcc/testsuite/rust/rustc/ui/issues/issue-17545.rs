#![feature(fn_traits)]

fn id<T>(x: T) -> T { x }

pub fn foo<'a, F: Fn(&'a ())>(bar: F) {
    bar.call((
        &id(()), // { dg-error ".E0716." "" { target *-*-* } }
    ));
}
fn main() {}

