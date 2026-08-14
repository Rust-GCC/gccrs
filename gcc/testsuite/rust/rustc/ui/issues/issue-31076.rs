#![feature(no_core, lang_items)]
#![no_core]

#[lang="sized"]
trait Sized {}

#[lang="add"]
trait Add<T> {}

impl Add<i32> for i32 {}

fn main() {
    let x = 5 + 6;
// { dg-error ".E0369." "" { target *-*-* } .-1 }
    let y = 5i32 + 6i32;
// { dg-error ".E0369." "" { target *-*-* } .-1 }
}

