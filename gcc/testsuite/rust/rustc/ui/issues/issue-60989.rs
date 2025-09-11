struct A {}
struct B {}

impl From<A> for B {
    fn from(a: A) -> B {
        B{}
    }
}

fn main() {
    let c1 = ();
    c1::<()>;
// { dg-error ".E0109." "" { target *-*-* } .-1 }

    let c1 = A {};
    c1::<dyn Into<B>>;
// { dg-error ".E0109." "" { target *-*-* } .-1 }
}

