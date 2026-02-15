// run-rustfix
fn main() {
    let _ = Option:Some(vec![0, 1]); // { dg-error "" "" { target *-*-* } }
}

