// Test that we do not consider parameter types to be sendable without
// an explicit trait bound.

fn foo<T>() {
    is_send::<T>() // { dg-error ".E0277." "" { target *-*-* } }
}

fn is_send<T:Send>() {
}

fn main() { }

