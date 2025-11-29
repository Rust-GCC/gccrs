// Issue #53114: NLL's borrow check had some deviations from the old borrow
// checker, and both had some deviations from our ideal state. This test
// captures the behavior of how `_` bindings are handled with respect to how we
// flag expressions that are meant to request unsafe blocks.

#![feature(untagged_unions)]

struct I(i64);
struct F(f64);

union U { a: I, b: F }

#[repr(packed)]
struct P {
    a: &'static i8,
    b: &'static u32,
}

fn let_wild_gets_unsafe_field() {
    let u1 = U { a: I(0) };
    let u2 = U { a: I(1) };
    let p = P { a: &2, b: &3 };
    let _ = &p.b;  // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
    let _ = u1.a;  // #53114: should eventually signal error as well
    let _ = &u2.a; // { dg-error ".E0133." "" { target *-*-* } }

    // variation on above with `_` in substructure
    let (_,) = (&p.b,);  // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
    let (_,) = (u1.a,);  // { dg-error ".E0133." "" { target *-*-* } }
    let (_,) = (&u2.a,); // { dg-error ".E0133." "" { target *-*-* } }
}

fn match_unsafe_field_to_wild() {
    let u1 = U { a: I(0) };
    let u2 = U { a: I(1) };
    let p = P { a: &2, b: &3 };
    match &p.b  { _ => { } } // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
    match u1.a  { _ => { } } // { dg-error ".E0133." "" { target *-*-* } }
    match &u2.a { _ => { } } // { dg-error ".E0133." "" { target *-*-* } }

    // variation on above with `_` in substructure
    match (&p.b,)  { (_,) => { } } // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
    match (u1.a,)  { (_,) => { } } // { dg-error ".E0133." "" { target *-*-* } }
    match (&u2.a,) { (_,) => { } } // { dg-error ".E0133." "" { target *-*-* } }
}

fn main() { }

