#![deny(irrefutable_let_patterns)]

fn main() {
    if let _ = 5 {} // { dg-error "" "" { target *-*-* } }

    while let _ = 5 { // { dg-error "" "" { target *-*-* } }
        break;
    }
}

