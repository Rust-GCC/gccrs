// Since `Y::B` here defaults to `Y::A+1`, this is also a
// recursive definition.
enum Y {
    A = Y::B as isize, // { dg-error ".E0391." "" { target *-*-* } }
    B,
}

fn main() { }

