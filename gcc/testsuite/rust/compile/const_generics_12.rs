// { dg-options "-w" }
#![feature(no_core)]
#![no_core]

#![feature(lang_items)]
#[lang = "sized"]
trait Sized {}

const BASE: usize = 2;

struct Foo<T, const N: usize> {
    data: [T; N],
}
struct Foo<const N: usize>;

fn main() {
    let _ = Foo::<u8, { BASE + 1 * 2 }> { data: [0; 4] };
    let _ = Foo::<{ 1 + 1 }>;
    // { dg-error "cannot evaluate constant expression" "" { target *-*-* } .-1 }
    // { dg-excess-errors "cascade errors from unresolved constant" }
}
