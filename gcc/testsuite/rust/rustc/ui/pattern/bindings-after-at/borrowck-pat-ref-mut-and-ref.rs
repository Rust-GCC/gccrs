#![feature(bindings_after_at)]

enum Option<T> {
    None,
    Some(T),
}

fn main() {
    match &mut Some(1) {
        ref mut z @ &mut Some(ref a) => {
// { dg-error ".E0502." "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }
            **z = None;
            println!("{}", *a);
        }
        _ => ()
    }

    struct U;

    // Prevent promotion:
    fn u() -> U { U }

    fn f1(ref a @ ref mut b: U) {}
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    fn f2(ref mut a @ ref b: U) {}
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    fn f3(ref a @ [ref b, ref mut mid @ .., ref c]: [U; 4]) {}
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    fn f4_also_moved(ref a @ ref mut b @ c: U) {}
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
// { dg-error ".E0382." "" { target *-*-* } .-3 }

    let ref mut a @ (ref b @ ref mut c) = u(); // sub-in-sub
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }

    let ref a @ ref mut b = U;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    let ref mut a @ ref b = U;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    let ref a @ (ref mut b, ref mut c) = (U, U);
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    let ref mut a @ (ref b, ref c) = (U, U);
// { dg-error ".E0502." "" { target *-*-* } .-1 }

    let ref mut a @ ref b = u();
// { dg-error ".E0502." "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }
    *a = u();
    drop(b);
    let ref a @ ref mut b = u();
// { dg-error ".E0502." "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }
    *b = u();
    drop(a);

    let ref mut a @ ref b = U;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    *a = U;
    drop(b);
    let ref a @ ref mut b = U;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    *b = U;
    drop(a);

    match Ok(U) {
        ref mut a @ Ok(ref b) | ref mut a @ Err(ref b) => {
// { dg-error ".E0502." "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }
            *a = Err(U);
            drop(b);
        }
    }

    match Ok(U) {
        ref a @ Ok(ref mut b) | ref a @ Err(ref mut b) => {
// { dg-error ".E0502." "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }
// { dg-error ".E0502." "" { target *-*-* } .-3 }
// { dg-error ".E0502." "" { target *-*-* } .-4 }
            *b = U;
            drop(a);
        }
    }

    match Ok(U) {
        ref a @ Ok(ref mut b) | ref a @ Err(ref mut b) if { *b = U; false } => {}
// { dg-error ".E0594." "" { target *-*-* } .-1 }
// { dg-error ".E0594." "" { target *-*-* } .-2 }
// { dg-error ".E0594." "" { target *-*-* } .-3 }
        _ => {}
    }
    match Ok(U) {
        ref mut a @ Ok(ref b) | ref mut a @ Err(ref b) if { *a = Err(U); false } => {}
// { dg-error ".E0502." "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }
// { dg-error ".E0594." "" { target *-*-* } .-3 }
        _ => {}
    }
    match Ok(U) {
        ref a @ Ok(ref mut b) | ref a @ Err(ref mut b) if { drop(b); false } => {}
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { dg-error ".E0507." "" { target *-*-* } .-2 }
// { dg-error ".E0507." "" { target *-*-* } .-3 }
// { dg-error ".E0507." "" { target *-*-* } .-4 }
        _ => {}
    }
    match Ok(U) {
        ref mut a @ Ok(ref b) | ref mut a @ Err(ref b) if { drop(a); false } => {}
// { dg-error ".E0502." "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }
// { dg-error ".E0507." "" { target *-*-* } .-3 }
// { dg-error ".E0507." "" { target *-*-* } .-4 }
        _ => {}
    }

    let ref a @ (ref mut b, ref mut c) = (U, U);
// { dg-error ".E0502." "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }
    *b = U;
    *c = U;

    let ref a @ (ref mut b, ref mut c) = (U, U);
// { dg-error ".E0502." "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }
    *b = U;
    drop(a);

    let ref a @ (ref mut b, ref mut c) = (U, U);
// { dg-error ".E0502." "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }
    *c = U;
    drop(a);
    let ref mut a @ (ref b, ref c) = (U, U);
// { dg-error ".E0502." "" { target *-*-* } .-1 }
}

