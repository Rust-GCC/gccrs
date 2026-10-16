enum Bug {
    V1 = return [0][0] // { dg-error ".E0572." "" { target *-*-* } }
}

fn main() {}

