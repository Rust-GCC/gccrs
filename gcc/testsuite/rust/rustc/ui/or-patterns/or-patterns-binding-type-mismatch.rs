// Here we test type checking of bindings when combined with or-patterns.
// Specifically, we ensure that introducing bindings of different types result in type errors.

#![feature(or_patterns)]

fn main() {
    enum Blah {
        A(isize, isize, usize),
        B(isize, isize),
    }

    match Blah::A(1, 1, 2) {
        Blah::A(_, x, y) | Blah::B(x, y) => {} // { dg-error ".E0308." "" { target *-*-* } }
    }

    match Some(Blah::A(1, 1, 2)) {
        Some(Blah::A(_, x, y) | Blah::B(x, y)) => {} // { dg-error ".E0308." "" { target *-*-* } }
    }

    match (0u8, 1u16) {
        (x, y) | (y, x) => {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    }

    match Some((0u8, Some((1u16, 2u32)))) {
        Some((x, Some((y, z)))) | Some((y, Some((x, z) | (z, x)))) => {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
        _ => {}
    }

    if let Blah::A(_, x, y) | Blah::B(x, y) = Blah::A(1, 1, 2) {
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    }

    if let Some(Blah::A(_, x, y) | Blah::B(x, y)) = Some(Blah::A(1, 1, 2)) {
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    }

    if let (x, y) | (y, x) = (0u8, 1u16) {
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    }

    if let Some((x, Some((y, z)))) | Some((y, Some((x, z) | (z, x))))
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
    = Some((0u8, Some((1u16, 2u32))))
    {}

    let Blah::A(_, x, y) | Blah::B(x, y) = Blah::A(1, 1, 2);
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    let (x, y) | (y, x) = (0u8, 1u16);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }

    fn f1((Blah::A(_, x, y) | Blah::B(x, y)): Blah) {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    fn f2(((x, y) | (y, x)): (u8, u16)) {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

