enum Bug<S> {
    Var = {
        let x: S = 0; // { dg-error ".E0308." "" { target *-*-* } }
        0
    },
}

fn main() {}

