macro_rules! black_hole {
    ($($tt:tt)*) => {}
}

fn main() {
    black_hole! { '\u{FFFFFF}' }
// { dg-error "" "" { target *-*-* } .-1 }
    black_hole! { "this is surrogate: \u{DAAA}" }
// { dg-error "" "" { target *-*-* } .-1 }
}

