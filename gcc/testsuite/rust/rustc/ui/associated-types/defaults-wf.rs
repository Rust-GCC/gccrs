// Check that associated type defaults are wf checked.

#![feature(associated_type_defaults)]

// Default types must always be wf
trait Tr3 {
    type Ty = Vec<[u8]>;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {}

