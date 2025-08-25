macro_rules! foo {
    ($a:expr) => a; // { dg-error "" "" { target *-*-* } }
}

fn main() {
    foo!(0); // Check that we report errors at macro definition, not expansion.

    let _: cfg!(foo) = (); // { dg-error "" "" { target *-*-* } }
}

