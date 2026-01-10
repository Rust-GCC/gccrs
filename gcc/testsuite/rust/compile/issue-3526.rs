// { dg-options "-w" }
// { dg-excess-errors "Noisy error reporting" }
extern "C" {
    fn some_func<T>(x: T); // { dg-error "foreign items may not have type parameters" }
    // { dg-message "replace the type parameters" "" { target *-*-* } .-1 }
}

fn main() {}
