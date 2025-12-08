extern "rust-intrinsic" {   // { dg-error ".E0658." "" { target *-*-* } }
    fn bar(); // { dg-error ".E0093." "" { target *-*-* } }
}

extern "rust-intrinsic" fn baz() {} // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn main() {}

