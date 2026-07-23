#![feature(lang_items)]

extern "C" {
    #[lang = "cake"]
    fn cake(); // { dg-error ".E0264." "" { target *-*-* } }
}

fn main() {}

