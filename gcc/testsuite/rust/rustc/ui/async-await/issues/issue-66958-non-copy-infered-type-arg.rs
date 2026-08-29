// edition:2018

struct Ia<S>(S);

impl<S> Ia<S> {
    fn partial(_: S) {}
    fn full(self) {}

    async fn crash(self) {
        Self::partial(self.0);
        Self::full(self); // { dg-error ".E0382." "" { target *-*-* } }
    }
}

fn main() {}

