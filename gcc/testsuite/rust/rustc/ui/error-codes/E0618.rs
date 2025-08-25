enum X {
    Entry,
}

fn main() {
    X::Entry();
// { dg-error ".E0618." "" { target *-*-* } .-1 }
    let x = 0i32;
    x();
// { dg-error ".E0618." "" { target *-*-* } .-1 }
}

