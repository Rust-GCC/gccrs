// check-pass
// aux-build:deprecated-macros.rs

#[macro_use] extern crate deprecated_macros;

#[deprecated(since = "1.0.0", note = "local deprecation note")]
#[macro_export]
macro_rules! local_deprecated{ () => () }

fn main() {
    local_deprecated!(); // { dg-warning "" "" { target *-*-* } }
    deprecated_macro!(); // { dg-warning "" "" { target *-*-* } }
}

