#![allow(unused)]
#![feature(lang_items)]

#[lang = "foo"]
fn bar() -> ! {
// { dg-error ".E0522." "" { target *-*-* } .-2 }
    loop {}
}

fn main() {}

