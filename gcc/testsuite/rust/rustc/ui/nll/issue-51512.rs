fn main() {
    let range = 0..1;
    let r = range;
    let x = range.start;
// { dg-error ".E0382." "" { target *-*-* } .-1 }
}

