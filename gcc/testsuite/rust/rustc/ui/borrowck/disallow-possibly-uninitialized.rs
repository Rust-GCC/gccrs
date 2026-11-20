// Test that we don't allow partial initialization.
// This may be relaxed in the future (see #54987).

fn main() {
    let mut t: (u64, u64);
    t.0 = 1;
// { dg-error ".E0381." "" { target *-*-* } .-1 }
    t.1 = 1;

    let mut t: (u64, u64);
    t.1 = 1;
// { dg-error ".E0381." "" { target *-*-* } .-1 }
    t.0 = 1;

    let mut t: (u64, u64);
    t.0 = 1;
// { dg-error ".E0381." "" { target *-*-* } .-1 }

    let mut t: (u64,);
    t.0 = 1;
// { dg-error ".E0381." "" { target *-*-* } .-1 }
}

