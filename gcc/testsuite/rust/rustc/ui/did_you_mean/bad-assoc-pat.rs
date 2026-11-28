fn main() {
    match 0u8 {
        [u8]::AssocItem => {}
// { dg-error ".E0599." "" { target *-*-* } .-1 }
// { dg-error ".E0599." "" { target *-*-* } .-2 }
        (u8, u8)::AssocItem => {}
// { dg-error ".E0599." "" { target *-*-* } .-1 }
// { dg-error ".E0599." "" { target *-*-* } .-2 }
        _::AssocItem => {}
// { dg-error ".E0599." "" { target *-*-* } .-1 }
// { dg-error ".E0599." "" { target *-*-* } .-2 }
    }
    match &0u8 {
        &(u8,)::AssocItem => {}
// { dg-error ".E0599." "" { target *-*-* } .-1 }
// { dg-error ".E0599." "" { target *-*-* } .-2 }
    }
}

macro_rules! pat {
    ($ty: ty) => ($ty::AssocItem)
// { dg-error ".E0599." "" { target *-*-* } .-1 }
// { dg-error ".E0599." "" { target *-*-* } .-2 }
}
macro_rules! ty {
    () => (u8)
}

fn check_macros() {
    match 0u8 {
        pat!(u8) => {}
        ty!()::AssocItem => {}
// { dg-error ".E0599." "" { target *-*-* } .-1 }
// { dg-error ".E0599." "" { target *-*-* } .-2 }
    }
}

