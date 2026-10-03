fn main() {
    let bar;
    fn baz(_x: isize) { }
    baz(bar); // { dg-error ".E0381." "" { target *-*-* } }
}

