struct S;

impl S {
    fn f() {}
}

macro_rules! impl_add {
    ($($n:ident)*) => {
        $(
            fn $n() {
                S::f::<i64>();
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }
            }
        )*
    }
}

impl_add!(a b);

fn main() {}

