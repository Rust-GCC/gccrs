macro_rules! foo {
    ($e: expr) => { 'x: loop { $e } }
}

pub fn main() {
    foo!(break 'x); // { dg-error ".E0426." "" { target *-*-* } }
}

