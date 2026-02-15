// aux-build:empty-struct.rs

extern crate empty_struct;

fn main() {
    let empty_struct::XEmpty2 = (); // { dg-error ".E0308." "" { target *-*-* } }
    let empty_struct::XEmpty6(..) = (); // { dg-error ".E0308." "" { target *-*-* } }
}

