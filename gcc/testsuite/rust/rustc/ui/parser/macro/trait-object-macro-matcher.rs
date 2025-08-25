// A single lifetime is not parsed as a type.
// `ty` matcher in particular doesn't accept a single lifetime

macro_rules! m {
    ($t: ty) => {
        let _: $t;
    };
}

fn main() {
    m!('static);
// { dg-error ".E0224." "" { target *-*-* } .-1 }
// { dg-error ".E0224." "" { target *-*-* } .-2 }
// { dg-warning ".E0224." "" { target *-*-* } .-3 }
}

