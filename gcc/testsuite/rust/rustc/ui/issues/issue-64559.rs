fn main() {
    let orig = vec![true];
    for _val in orig {}
    let _closure = || orig;
// { dg-error ".E0382." "" { target *-*-* } .-1 }
}

