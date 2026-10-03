fn main() {
    let mut v: Vec<()> = Vec::new();
    || &mut v;
// { dg-error "" "" { target *-*-* } .-1 }
}

