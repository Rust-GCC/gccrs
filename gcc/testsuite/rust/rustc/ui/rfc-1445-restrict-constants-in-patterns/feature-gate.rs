// Test that use of structural-match traits is only permitted with a feature gate,
// and that if a feature gate is supplied, it permits the type to be
// used in a match.

// revisions: with_gate no_gate

// gate-test-structural_match

#![allow(unused)]
#![feature(rustc_attrs)]
#![cfg_attr(with_gate, feature(structural_match))]


struct Foo {
    x: u32
}

const FOO: Foo = Foo { x: 0 };

#[rustc_error]
fn main() { // { dg-error "" "" { target *-*-* } }
    let y = Foo { x: 1 };
    match y {
        FOO => { }
        _ => { }
    }
}

impl std::marker::StructuralPartialEq for Foo { }
// { dg-error "" "" { target *-*-* } .-1 }
impl std::marker::StructuralEq for Foo { }
// { dg-error "" "" { target *-*-* } .-1 }

impl PartialEq<Foo> for Foo {
    fn eq(&self, other: &Self) -> bool {
        self.x == other.x
    }
}
impl Eq for Foo { }

