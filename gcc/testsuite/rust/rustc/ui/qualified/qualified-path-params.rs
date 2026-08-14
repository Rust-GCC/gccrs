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

fn main() {
    match 10 {
        <S as Tr>::A::f::<u8> => {}
// { dg-error ".E0533." "" { target *-*-* } .-1 }
        0 ..= <S as Tr>::A::f::<u8> => {}
// { dg-error ".E0029." "" { target *-*-* } .-1 }
    }
}

