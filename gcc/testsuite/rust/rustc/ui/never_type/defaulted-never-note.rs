// We need to opt into the `never_type_fallback` feature
// to trigger the requirement that this is testing.
#![feature(never_type, never_type_fallback)]

#![allow(unused)]

trait Deserialize: Sized {
    fn deserialize() -> Result<Self, String>;
}

impl Deserialize for () {
    fn deserialize() -> Result<(), String> {
        Ok(())
    }
}

trait ImplementedForUnitButNotNever {}

impl ImplementedForUnitButNotNever for () {}

fn foo<T: ImplementedForUnitButNotNever>(_t: T) {}
// { dg-note "" "" { target *-*-* } .-1 }

fn smeg() {
    let _x = return;
    foo(_x);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-note ".E0277." "" { target *-*-* } .-2 }
// { dg-note ".E0277." "" { target *-*-* } .-3 }
}

fn main() {
    smeg();
}

