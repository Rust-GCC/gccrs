// Ensure assert macro does not ignore trailing garbage.
//
// See https://github.com/rust-lang/rust/issues/60024 for details.

fn main() {
    assert!(true some extra junk, "whatever");
// { dg-error "" "" { target *-*-* } .-1 }

    assert!(true some extra junk);
// { dg-error "" "" { target *-*-* } .-1 }

    assert!(true, "whatever" blah);
// { dg-error "" "" { target *-*-* } .-1 }

    assert!(true "whatever" blah);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    assert!(true;);
// { dg-error "" "" { target *-*-* } .-1 }

    assert!(false || true "error message");
// { dg-error "" "" { target *-*-* } .-1 }
}

