// Check that we do not allow coercions to object
// unsafe trait objects in match arms

#![feature(object_safe_for_dispatch)]

trait Trait: Sized {}

struct S;

impl Trait for S {}

struct R;

impl Trait for R {}

fn opt() -> Option<()> {
    Some(())
}

fn main() {
    match opt() {
        Some(()) => &S,
        None => &R,  // { dg-error ".E0308." "" { target *-*-* } }
    }
    let t: &dyn Trait = match opt() { // { dg-error ".E0038." "" { target *-*-* } }
        Some(()) => &S, // { dg-error ".E0038." "" { target *-*-* } }
        None => &R,
    };
}

