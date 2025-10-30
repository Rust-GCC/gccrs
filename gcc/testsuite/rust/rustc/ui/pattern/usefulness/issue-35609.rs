enum Enum {
    A, B, C, D, E, F
}
use Enum::*;

struct S(Enum, ());
struct Sd { x: Enum, y: () }

fn main() {
    match (A, ()) { // { dg-error ".E0004." "" { target *-*-* } }
        (A, _) => {}
    }

    match (A, A) { // { dg-error ".E0004." "" { target *-*-* } }
        (_, A) => {}
    }

    match ((A, ()), ()) { // { dg-error ".E0004." "" { target *-*-* } }
        ((A, ()), _) => {}
    }

    match ((A, ()), A) { // { dg-error ".E0004." "" { target *-*-* } }
        ((A, ()), _) => {}
    }

    match ((A, ()), ()) { // { dg-error ".E0004." "" { target *-*-* } }
        ((A, _), _) => {}
    }


    match S(A, ()) { // { dg-error ".E0004." "" { target *-*-* } }
        S(A, _) => {}
    }

    match (Sd { x: A, y: () }) { // { dg-error ".E0004." "" { target *-*-* } }
        Sd { x: A, y: _ } => {}
    }

    match Some(A) { // { dg-error ".E0004." "" { target *-*-* } }
        Some(A) => (),
        None => ()
    }
}

