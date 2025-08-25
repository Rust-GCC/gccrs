macro_rules! foo {
    ($d:expr) => {{
        fn bar(d: u8) { }
        bar(&mut $d);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    }}
}

fn main() {
    foo!(0u8);
// { dg-error "" "" { target *-*-* } .-1 }
}

