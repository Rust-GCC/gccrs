#![deny(unused_must_use)]

fn foo() -> (Result<(), ()>, ()) {
    (Ok::<(), ()>(()), ())
}

fn main() {
    (Ok::<(), ()>(()),); // { dg-error "" "" { target *-*-* } }

    (Ok::<(), ()>(()), 0, Ok::<(), ()>(()), 5);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    foo(); // { dg-error "" "" { target *-*-* } }

    ((Err::<(), ()>(()), ()), ()); // { dg-error "" "" { target *-*-* } }
}

