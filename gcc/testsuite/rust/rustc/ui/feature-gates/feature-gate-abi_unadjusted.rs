extern "unadjusted" fn foo() {
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

fn main() {
    foo();
}

