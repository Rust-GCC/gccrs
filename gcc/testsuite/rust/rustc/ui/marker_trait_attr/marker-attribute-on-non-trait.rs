#![feature(marker_trait_attr)]

#[marker] // { dg-error "" "" { target *-*-* } }
struct Struct {}

#[marker] // { dg-error "" "" { target *-*-* } }
impl Struct {}

#[marker] // { dg-error "" "" { target *-*-* } }
union Union {
    x: i32,
}

#[marker] // { dg-error "" "" { target *-*-* } }
const CONST: usize = 10;

#[marker] // { dg-error "" "" { target *-*-* } }
fn function() {}

#[marker] // { dg-error "" "" { target *-*-* } }
type Type = ();

fn main() {}

