// run-rustfix
struct X {}
fn main() {
    let _ = vec![X]; //…
// { dg-error ".E0423." "" { target *-*-* } .-1 }
}

