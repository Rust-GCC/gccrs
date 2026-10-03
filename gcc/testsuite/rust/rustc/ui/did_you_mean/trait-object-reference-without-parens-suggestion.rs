#![allow(bare_trait_objects)]

fn main() {
    let _: &Copy + 'static; // { dg-error ".E0038." "" { target *-*-* } }
// { dg-error ".E0038." "" { target *-*-* } .-1 }
    let _: &'static Copy + 'static; // { dg-error ".E0178." "" { target *-*-* } }
}

