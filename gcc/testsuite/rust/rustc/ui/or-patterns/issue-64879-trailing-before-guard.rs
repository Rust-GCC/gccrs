// In this regression test we check that a trailing `|` in an or-pattern just
// before the `if` token of a `match` guard will receive parser recovery with
// an appropriate error message.

enum E { A, B }

fn main() {
    match E::A {
        E::A |
        E::B | // { dg-error "" "" { target *-*-* } }
        if true => {
            let recovery_witness: bool = 0; // { dg-error ".E0308." "" { target *-*-* } }
        }
    }
}

