fn main() {}

extern "C" {
    default!(); // { dg-error "" "" { target *-*-* } }
    default do
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

