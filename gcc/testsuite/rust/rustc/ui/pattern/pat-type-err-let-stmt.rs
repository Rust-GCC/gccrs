// Test the `.span_label` to the type / scrutinee
// when there's a type error in checking a pattern.

fn main() {
    // We want to point at the `Option<u8>`.
    let Ok(0): Option<u8> = 42u8;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }

    // We want to point at the `Option<u8>`.
    let Ok(0): Option<u8>;
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    // We want to point at the scrutinee.
    let Ok(0) = 42u8; // { dg-error ".E0308." "" { target *-*-* } }
}

