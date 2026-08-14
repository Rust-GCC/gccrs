// Test that `binding @ subpat` acts as a product context with respect to duplicate binding names.
// The code that is tested here lives in resolve (see `resolve_pattern_inner`).

#![feature(bindings_after_at)]
#![feature(or_patterns)]

fn main() {
    fn f(a @ a @ a: ()) {}
// { dg-error ".E0415." "" { target *-*-* } .-1 }
// { dg-error ".E0415." "" { target *-*-* } .-2 }

    match Ok(0) {
        Ok(a @ b @ a)
// { dg-error ".E0416." "" { target *-*-* } .-1 }
        | Err(a @ b @ a)
// { dg-error ".E0416." "" { target *-*-* } .-1 }
        => {}
    }

    let a @ a @ a = ();
// { dg-error ".E0416." "" { target *-*-* } .-1 }
// { dg-error ".E0416." "" { target *-*-* } .-2 }
    let ref a @ ref a = ();
// { dg-error ".E0416." "" { target *-*-* } .-1 }
    let ref mut a @ ref mut a = ();
// { dg-error ".E0416." "" { target *-*-* } .-1 }

    let a @ (Ok(a) | Err(a)) = Ok(());
// { dg-error ".E0416." "" { target *-*-* } .-1 }
// { dg-error ".E0416." "" { target *-*-* } .-2 }
}

