// Make sure speculative path resolution works properly when resolution
// adjustment happens and no extra errors is reported.

struct S {
    field: u8,
}

trait Tr {
    fn method(&self);
}

impl Tr for S {
    fn method(&self) {
        fn g() {
            // Speculative resolution of `Self` and `self` silently fails,
            // "did you mean" messages are not printed.
            field;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
            method();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        }

        field;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        method();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }
}

fn main() {}

