#![feature(box_patterns)]
#![feature(box_syntax)]

enum A { B, C }

fn main() {
    match (true, false) {
        A::B => (),
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
        _ => ()
    }

    match (true, false) {
        (true, false, false) => ()
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
    }

    match (true, false) {
        (true, false, false) => ()
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
    }

    match (true, false) {
        box (true, false) => ()
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
    }

    match (true, false) {
        &(true, false) => ()
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
    }


    let v = [('a', 'b')   // { dg-error ".E0618." "" { target *-*-* } }
             ('c', 'd'),
             ('e', 'f')];

    for &(x,y) in &v {} // should be OK

    // Make sure none of the errors above were fatal
    let x: char = true; // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
}

