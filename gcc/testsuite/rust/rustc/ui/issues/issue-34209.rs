enum S {
    A,
}

fn bug(l: S) {
    match l {
        S::B {} => {}, // { dg-error ".E0599." "" { target *-*-* } }
    }
}

fn main () {}

