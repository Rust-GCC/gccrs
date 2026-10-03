// Test static calls to make sure that we align the Self and input
// type parameters on a trait correctly.

trait Tr<T> : Sized {
    fn op(_: T) -> Self;
}

trait A:    Tr<Self> {
    fn test<U>(u: U) -> Self {
        Tr::op(u)   // { dg-error ".E0277." "" { target *-*-* } }
    }
}

trait B<T>: Tr<T> {
    fn test<U>(u: U) -> Self {
        Tr::op(u)   // { dg-error ".E0277." "" { target *-*-* } }
    }
}

fn main() {
}

