pub struct GenX<S> {
    inner: S,
}

impl<S> Into<S> for GenX<S> { // { dg-error ".E0119." "" { target *-*-* } }
    fn into(self) -> S {
        self.inner
    }
}

fn main() {}

