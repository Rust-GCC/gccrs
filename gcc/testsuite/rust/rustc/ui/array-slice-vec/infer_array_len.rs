// see issue #70529
struct A;

impl From<A> for [u8; 2] {
    fn from(a: A) -> Self {
        [0; 2]
    }
}

impl From<A> for [u8; 3] {
    fn from(a: A) -> Self {
        [0; 3]
    }
}


fn main() {
    let a = A;
    let [_, _] = a.into();
// { dg-error ".E0282." "" { target *-*-* } .-1 }
}

