fn main() {
    let mut v = vec![Some("foo"), Some("bar")];
    v.push(v.pop().unwrap()); // { dg-error ".E0499." "" { target *-*-* } }
}

