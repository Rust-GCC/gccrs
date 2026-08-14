pub struct Foo<A, B>(A, B);

impl<A, B> Foo<A, B> {
    const HOST_SIZE: usize = std::mem::size_of::<B>();

    pub fn crash() -> bool {
        [5; Self::HOST_SIZE] == [6; 0]
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }
    }
}

fn main() {}

