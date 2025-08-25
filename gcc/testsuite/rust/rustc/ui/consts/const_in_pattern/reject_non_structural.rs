// This test of structural match checking enumerates the different kinds of
// const definitions, collecting cases where the const pattern is rejected.
//
// Note: Even if a non-structural-match type is part of an expression in a
// const's definition, that does not necessarily disqualify the const from being
// a match pattern: in principle, we just need the types involved in the final
// value to be structurally matchable.

// See also RFC 1445

#![feature(type_ascription)]
#![warn(indirect_structural_match)]
// { dg-note "" "" { target *-*-* } .-1 }

#[derive(Copy, Clone, Debug)]
struct NoPartialEq;

#[derive(Copy, Clone, Debug)]
struct NoDerive;

// This impl makes `NoDerive` irreflexive.
impl PartialEq for NoDerive { fn eq(&self, _: &Self) -> bool { false } }

impl Eq for NoDerive { }

type OND = Option<NoDerive>;

struct TrivialEq(OND);

// This impl makes `TrivialEq` trivial.
impl PartialEq for TrivialEq { fn eq(&self, _: &Self) -> bool { true } }

impl Eq for TrivialEq { }

fn main() {
    #[derive(PartialEq, Eq, Debug)]
    enum Derive<X> { Some(X), None, }

    const ENUM: Derive<NoDerive> = Derive::Some(NoDerive);
    match Derive::Some(NoDerive) { ENUM => dbg!(ENUM), _ => panic!("whoops"), };
// { dg-error "" "" { target *-*-* } .-1 }

    const FIELD: OND = TrivialEq(Some(NoDerive)).0;
    match Some(NoDerive) { FIELD => dbg!(FIELD), _ => panic!("whoops"), };
// { dg-error "" "" { target *-*-* } .-1 }

    const NO_DERIVE_SOME: OND = Some(NoDerive);
    const INDIRECT: OND = NO_DERIVE_SOME;
    match Some(NoDerive) {INDIRECT => dbg!(INDIRECT), _ => panic!("whoops"), };
// { dg-error "" "" { target *-*-* } .-1 }

    const TUPLE: (OND, OND) = (None, Some(NoDerive));
    match (None, Some(NoDerive)) { TUPLE => dbg!(TUPLE), _ => panic!("whoops"), };
// { dg-error "" "" { target *-*-* } .-1 }

    const TYPE_ASCRIPTION: OND = Some(NoDerive): OND;
    match Some(NoDerive) { TYPE_ASCRIPTION => dbg!(TYPE_ASCRIPTION), _ => panic!("whoops"), };
// { dg-error "" "" { target *-*-* } .-1 }

    const ARRAY: [OND; 2] = [None, Some(NoDerive)];
    match [None, Some(NoDerive)] { ARRAY => dbg!(ARRAY), _ => panic!("whoops"), };
// { dg-error "" "" { target *-*-* } .-1 }

    const REPEAT: [OND; 2] = [Some(NoDerive); 2];
    match [Some(NoDerive); 2] { REPEAT => dbg!(REPEAT), _ => panic!("whoops"), };
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    trait Trait: Sized { const ASSOC: Option<Self>; }
    impl Trait for NoDerive { const ASSOC: Option<NoDerive> = Some(NoDerive); }
    match Some(NoDerive) { NoDerive::ASSOC => dbg!(NoDerive::ASSOC), _ => panic!("whoops"), };
// { dg-error "" "" { target *-*-* } .-1 }

    const BLOCK: OND = { NoDerive; Some(NoDerive) };
    match Some(NoDerive) { BLOCK => dbg!(BLOCK), _ => panic!("whoops"), };
// { dg-error "" "" { target *-*-* } .-1 }

    const ADDR_OF: &OND = &Some(NoDerive);
    match &Some(NoDerive) { ADDR_OF => dbg!(ADDR_OF), _ => panic!("whoops"), };
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
}

