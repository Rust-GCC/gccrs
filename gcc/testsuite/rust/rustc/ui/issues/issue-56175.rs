// edition:2018
// aux-crate:reexported_trait=reexported-trait.rs

fn main() {
    reexported_trait::FooStruct.trait_method();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    reexported_trait::FooStruct.trait_method_b();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

