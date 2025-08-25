// Check that qualified paths with type parameters
// fail during type checking and not during parsing

struct S;

trait Tr {
    type A;
}

impl Tr for S {
    type A = S;
}

impl S {
    fn f<T>() {}
}

type A = <S as Tr>::A::f<u8>;
// { dg-error ".E0223." "" { target *-*-* } .-1 }

fn main() {}

