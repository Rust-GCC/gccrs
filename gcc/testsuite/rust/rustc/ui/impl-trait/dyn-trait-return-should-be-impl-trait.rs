#![allow(bare_trait_objects)]
struct Struct;
trait Trait {}
impl Trait for Struct {}
impl Trait for u32 {}

fn fuz() -> (usize, Trait) { (42, Struct) }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
fn bar() -> (usize, dyn Trait) { (42, Struct) }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
fn bap() -> Trait { Struct }
// { dg-error ".E0746." "" { target *-*-* } .-1 }
fn ban() -> dyn Trait { Struct }
// { dg-error ".E0746." "" { target *-*-* } .-1 }
fn bak() -> dyn Trait { unimplemented!() } // { dg-error ".E0746." "" { target *-*-* } }
// Suggest using `Box<dyn Trait>`
fn bal() -> dyn Trait { // { dg-error ".E0746." "" { target *-*-* } }
    if true {
        return Struct;
    }
    42
}
fn bax() -> dyn Trait { // { dg-error ".E0746." "" { target *-*-* } }
    if true {
        Struct
    } else {
        42 // { dg-error ".E0308." "" { target *-*-* } }
    }
}
fn bam() -> Box<dyn Trait> {
    if true {
        return Struct; // { dg-error ".E0308." "" { target *-*-* } }
    }
    42 // { dg-error ".E0308." "" { target *-*-* } }
}
fn baq() -> Box<dyn Trait> {
    if true {
        return 0; // { dg-error ".E0308." "" { target *-*-* } }
    }
    42 // { dg-error ".E0308." "" { target *-*-* } }
}
fn baz() -> Box<dyn Trait> {
    if true {
        Struct // { dg-error ".E0308." "" { target *-*-* } }
    } else {
        42 // { dg-error ".E0308." "" { target *-*-* } }
    }
}
fn baw() -> Box<dyn Trait> {
    if true {
        0 // { dg-error ".E0308." "" { target *-*-* } }
    } else {
        42 // { dg-error ".E0308." "" { target *-*-* } }
    }
}

// Suggest using `impl Trait`
fn bat() -> dyn Trait { // { dg-error ".E0746." "" { target *-*-* } }
    if true {
        return 0;
    }
    42
}
fn bay() -> dyn Trait { // { dg-error ".E0746." "" { target *-*-* } }
    if true {
        0
    } else {
        42
    }
}

fn main() {}

