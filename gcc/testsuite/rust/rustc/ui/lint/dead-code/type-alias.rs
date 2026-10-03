#![deny(dead_code)]

type Used = u8;
type Unused = u8; // { dg-error "" "" { target *-*-* } }

fn id(x: Used) -> Used { x }

fn main() {
    id(0);
}

