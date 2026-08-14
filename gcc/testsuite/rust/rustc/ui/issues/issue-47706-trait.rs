trait T {
    fn f(&self, _: ()) {
        None::<()>.map(Self::f);
    }
// { dg-error ".E0593." "" { target *-*-* } .-2 }
}

fn main() {}

