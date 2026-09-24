#![feature(non_ascii_idents)]
#![deny(mixed_script_confusables)]

struct ΑctuallyNotLatin;
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
    let v = ΑctuallyNotLatin;
}

mod роре {
// { dg-error "" "" { target *-*-* } .-1 }
    const エ: &'static str = "アイウ";
// { dg-error "" "" { target *-*-* } .-1 }
}

