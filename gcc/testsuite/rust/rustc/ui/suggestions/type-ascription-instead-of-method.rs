// run-rustfix
fn main() {
    let _ = Box:new("foo".to_string());
// { dg-error "" "" { target *-*-* } .-1 }
}

