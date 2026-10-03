// run-rustfix
// Make sure that invalid ranges generate an error during parsing, not an ICE

#![allow(path_statements)]

pub fn main() {
    ..;
    0..;
    ..1;
    0..1;
    ..=; // { dg-error ".E0586." "" { target *-*-* } }
// { help ".E0586." "" { target *-*-* } .-1 }
}

fn _foo1() {
    ..=1;
    0..=1;
    0..=; // { dg-error ".E0586." "" { target *-*-* } }
// { help ".E0586." "" { target *-*-* } .-1 }
}

