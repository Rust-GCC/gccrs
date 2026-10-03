mod foo {
    default!(); // OK.
    default do
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

