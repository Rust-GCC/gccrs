trait Dim {
    fn dim() -> usize;
}

enum Dim3 {}

impl Dim for Dim3 {
    fn dim() -> usize {
        3
    }
}

pub struct Vector<T, D: Dim> {
    entries: [T; D::dim()],
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    _dummy: D,
}

fn main() {}

