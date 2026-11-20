// run-rustfix
fn main() {
    let _ = Option:Some("");
// { dg-error "" "" { target *-*-* } .-1 }
}

