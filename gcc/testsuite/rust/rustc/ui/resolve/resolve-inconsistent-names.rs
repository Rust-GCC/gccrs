#![allow(non_camel_case_types)]

enum E { A, B, c }

mod m {
    const CONST1: usize = 10;
    const Const2: usize = 20;
}

fn main() {
    let y = 1;
    match y {
       a | b => {} // { dg-error ".E0408." "" { target *-*-* } }
// { dg-error ".E0408." "" { target *-*-* } .-2 }
    }

    let x = (E::A, E::B);
    match x {
        (A, B) | (ref B, c) | (c, A) => ()
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
// { dg-error ".E0308." "" { target *-*-* } .-5 }
// { help ".E0308." "" { target *-*-* } .-6 }
    }

    let z = (10, 20);
    match z {
        (CONST1, _) | (_, Const2) => ()
// { dg-error ".E0408." "" { target *-*-* } .-1 }
// { help ".E0408." "" { target *-*-* } .-2 }
// { dg-error ".E0408." "" { target *-*-* } .-3 }
// { help ".E0408." "" { target *-*-* } .-4 }
    }
}

