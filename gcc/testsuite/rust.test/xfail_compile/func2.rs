// { dg-excess-errors "Noisy error and debug" }
fn test(a: i32, b: i32) -> i32 {
    a + b
}

fn main() {
    let a = test(1); // { dg-error "unexpected number of arguments 1 expected 2"
    // { dg-error "failed to lookup type to CallExpr" "" { target { *-*-* } } .-1 }
    // { dg-error "failed to type resolve expression" "" { target { *-*-* } } .-2 }
}
