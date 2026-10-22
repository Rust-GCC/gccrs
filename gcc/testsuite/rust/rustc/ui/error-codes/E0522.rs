#![feature(lang_items)]

#[lang = "cookie"]
fn cookie() -> ! {
// { dg-error ".E0522." "" { target *-*-* } .-2 }
    loop {}
}

fn main() {}

