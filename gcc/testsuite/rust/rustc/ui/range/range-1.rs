// Test range syntax - type errors.

pub fn main() {
    // Mixed types.
    let _ = 0u32..10i32;
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    // Bool => does not implement iterator.
    for i in false..true {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    // Unsized type.
    let arr: &[_] = &[1, 2, 3];
    let range = *arr..;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

