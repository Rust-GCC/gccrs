// Test that we mark enum discriminant values as having errors, even when the
// diagnostic is deduplicated.

struct F;
struct T;

impl F {
    const V: i32 = 0;
}

impl T {
    const V: i32 = 0;
}

macro_rules! mac {
    ($( $v: ident = $s: ident,)*) => {
        enum E {
            $( $v = $s::V, )*
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
        }
    }
}

mac! {
    A = F,
    B = T,
}

fn main() {}

