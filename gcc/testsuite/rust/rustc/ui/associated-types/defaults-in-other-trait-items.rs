#![feature(associated_type_defaults)]

// Associated type defaults may not be assumed inside the trait defining them.
// ie. they only resolve to `<Self as Tr>::A`, not the actual type `()`
trait Tr {
    type A = (); // { dg-note "" "" { target *-*-* } }

    fn f(p: Self::A) {
        let () = p;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-note ".E0308." "" { target *-*-* } .-2 }
// { dg-note ".E0308." "" { target *-*-* } .-3 }
    }
}

// An impl that doesn't override the type *can* assume the default.
impl Tr for () {
    fn f(p: Self::A) {
        let () = p;
    }
}

impl Tr for u8 {
    type A = ();

    fn f(p: Self::A) {
        let () = p;
    }
}

trait AssocConst {
    type Ty = u8; // { dg-note "" "" { target *-*-* } }

    // Assoc. consts also cannot assume that default types hold
    const C: Self::Ty = 0u8;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-note ".E0308." "" { target *-*-* } .-2 }
// { dg-note ".E0308." "" { target *-*-* } .-3 }
}

// An impl can, however
impl AssocConst for () {
    const C: Self::Ty = 0u8;
}

fn main() {}

