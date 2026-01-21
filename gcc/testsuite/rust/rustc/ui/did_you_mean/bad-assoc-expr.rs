fn main() {
    let a = [1, 2, 3, 4];
    [i32; 4]::clone(&a);
// { dg-error "" "" { target *-*-* } .-1 }

    [i32]::as_ref(&a);
// { dg-error "" "" { target *-*-* } .-1 }

    (u8)::clone(&0);
// { dg-error "" "" { target *-*-* } .-1 }

    (u8, u8)::clone(&(0, 0));
// { dg-error "" "" { target *-*-* } .-1 }

    &(u8)::clone(&0);
// { dg-error "" "" { target *-*-* } .-1 }

    10 + (u8)::clone(&0);
// { dg-error "" "" { target *-*-* } .-1 }
}

macro_rules! expr {
    ($ty: ty) => ($ty::clone(&0))
// { dg-error "" "" { target *-*-* } .-1 }
}
macro_rules! ty {
    () => (u8)
}

fn check_macros() {
    expr!(u8);
    let _ = ty!()::clone(&0);
// { dg-error "" "" { target *-*-* } .-1 }
    ty!()::clone(&0);
// { dg-error "" "" { target *-*-* } .-1 }
}

