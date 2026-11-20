macro_rules! foo {
    ($e: expr) => { 'x: for _ in 0..1 { $e } }
}

pub fn main() {
    foo!(break 'x); // { dg-error ".E0426." "" { target *-*-* } }
}

