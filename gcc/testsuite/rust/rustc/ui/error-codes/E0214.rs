fn main() {
    let v: Vec(&str) = vec!["foo"];
// { dg-error ".E0214." "" { target *-*-* } .-1 }
}

