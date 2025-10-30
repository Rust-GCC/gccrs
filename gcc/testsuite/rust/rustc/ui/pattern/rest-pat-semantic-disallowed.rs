// Here we test that rest patterns, i.e. `..`, are not allowed
// outside of slice (+ ident patterns witin those), tuple,
// and tuple struct patterns and that duplicates are caught in these contexts.

#![feature(box_patterns)]

fn main() {}

macro_rules! mk_pat {
    () => { .. } // { dg-error "" "" { target *-*-* } }
}

fn rest_patterns() {
    let mk_pat!();

    // Top level:
    fn foo(..: u8) {} // { dg-error "" "" { target *-*-* } }
    let ..;  // { dg-error "" "" { target *-*-* } }

    // Box patterns:
    let box ..;  // { dg-error "" "" { target *-*-* } }

    // In or-patterns:
    match 1 {
        1 | .. => {} // { dg-error "" "" { target *-*-* } }
    }

    // Ref patterns:
    let &..; // { dg-error "" "" { target *-*-* } }
    let &mut ..; // { dg-error "" "" { target *-*-* } }

    // Ident patterns:
    let x @ ..; // { dg-error ".E0282." "" { target *-*-* } }
// { dg-error ".E0282." "" { target *-*-* } .-1 }
    let ref x @ ..; // { dg-error "" "" { target *-*-* } }
    let ref mut x @ ..; // { dg-error "" "" { target *-*-* } }

    // Tuple:
    let (..): (u8,); // OK.
    let (..,): (u8,); // OK.
    let (
        ..,
        .., // { dg-error "" "" { target *-*-* } }
        .. // { dg-error "" "" { target *-*-* } }
    ): (u8, u8, u8);
    let (
        ..,
        x,
        .. // { dg-error "" "" { target *-*-* } }
    ): (u8, u8, u8);

    struct A(u8, u8, u8);

    // Tuple struct (same idea as for tuple patterns):
    let A(..); // OK.
    let A(..,); // OK.
    let A(
        ..,
        .., // { dg-error "" "" { target *-*-* } }
        .. // { dg-error "" "" { target *-*-* } }
    );
    let A(
        ..,
        x,
        .. // { dg-error "" "" { target *-*-* } }
    );

    // Array/Slice:
    let [..]: &[u8]; // OK.
    let [..,]: &[u8]; // OK.
    let [
        ..,
        .., // { dg-error "" "" { target *-*-* } }
        .. // { dg-error "" "" { target *-*-* } }
    ]: &[u8];
    let [
        ..,
        ref x @ .., // { dg-error "" "" { target *-*-* } }
        ref mut y @ .., // { dg-error "" "" { target *-*-* } }
        (ref z @ ..), // { dg-error "" "" { target *-*-* } }
        .. // { dg-error "" "" { target *-*-* } }
    ]: &[u8];
}

