struct S;

impl S {
    fn foo(&self, &str bar) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }
}

fn baz(S quux, xyzzy: i32) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }

fn one(i32 a b) {}
// { dg-error "" "" { target *-*-* } .-1 }

fn pattern((i32, i32) (a, b)) {}
// { dg-error "" "" { target *-*-* } .-1 }

fn fizz(i32) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }
// { help "" "" { target *-*-* } .-4 }

fn missing_colon(quux S) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }

fn main() {}

