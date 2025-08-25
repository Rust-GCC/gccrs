// aux-build:xc-private-method-lib.rs

extern crate xc_private_method_lib;

fn main() {
    let _ = xc_private_method_lib::Struct::static_meth_struct();
// { dg-error ".E0624." "" { target *-*-* } .-1 }

    let _ = xc_private_method_lib::Enum::static_meth_enum();
// { dg-error ".E0624." "" { target *-*-* } .-1 }
}

