macro_rules! foo {
    () => { break 'x; } // { dg-error ".E0426." "" { target *-*-* } }
}

pub fn main() {
    'x: loop { foo!() }
}

