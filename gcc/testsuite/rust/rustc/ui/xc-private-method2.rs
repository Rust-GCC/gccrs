// aux-build:xc-private-method-lib.rs

extern crate xc_private_method_lib;

fn main() {
    let _ = xc_private_method_lib::Struct{ x: 10 }.meth_struct();
// { dg-error ".E0624." "" { target *-*-* } .-1 }

    let _ = xc_private_method_lib::Enum::Variant1(20).meth_enum();
// { dg-error ".E0624." "" { target *-*-* } .-1 }
}

