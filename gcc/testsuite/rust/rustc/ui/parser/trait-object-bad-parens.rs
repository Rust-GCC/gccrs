#![feature(optin_builtin_traits)]
#![feature(negative_impls)]
#![allow(bare_trait_objects)]

auto trait Auto {}

fn main() {
    let _: Box<((Auto)) + Auto>;
// { dg-error ".E0178." "" { target *-*-* } .-1 }
    let _: Box<(Auto + Auto) + Auto>;
// { dg-error ".E0178." "" { target *-*-* } .-1 }
    let _: Box<(Auto +) + Auto>;
// { dg-error ".E0178." "" { target *-*-* } .-1 }
    let _: Box<(dyn Auto) + Auto>;
// { dg-error ".E0178." "" { target *-*-* } .-1 }
}

