pub trait ToNbt<T> {
    fn new(val: T) -> Self;
}

impl dyn ToNbt<Self> {}
// { dg-error ".E0391." "" { target *-*-* } .-1 }

fn main() {}

