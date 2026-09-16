pub struct Gcm<E>(E);

impl<E> Gcm<E> {
    pub fn crash(e: E) -> Self {
        Self::<E>(e)
// { dg-error ".E0109." "" { target *-*-* } .-1 }
    }
}

fn main() {}

