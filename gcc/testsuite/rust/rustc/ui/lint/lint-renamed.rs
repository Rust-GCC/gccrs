#[deny(bare_trait_object)]
// { dg-warning "" "" { target *-*-* } .-1 }
#[deny(unused)]
fn main() { let unused = (); } // { dg-error "" "" { target *-*-* } }

