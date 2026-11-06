// { dg-output "f32 neg one\r*\nf32 zero\r*\nf32 zero\r*\nf32 one\r*\nf32 other\r*\n" }
// Tests f32 floating-point literal pattern matching, including
// that -0.0 and 0.0 are treated as equal (== semantics, not bitwise).
#![feature(no_core)]
#![no_core]

extern "C" {
    fn printf(s: *const i8, ...);
}

fn classify_f32(x: f32) {
    match x {
        -1.0 => unsafe {
            let a = "f32 neg one\n\0";
            let b = a as *const str;
            let c = b as *const i8;
            printf(c);
        },
        0.0 => unsafe {
            let a = "f32 zero\n\0";
            let b = a as *const str;
            let c = b as *const i8;
            printf(c);
        },
        1.0 => unsafe {
            let a = "f32 one\n\0";
            let b = a as *const str;
            let c = b as *const i8;
            printf(c);
        },
        _ => unsafe {
            let a = "f32 other\n\0";
            let b = a as *const str;
            let c = b as *const i8;
            printf(c);
        },
    }
}

fn main() -> i32 {
    let neg_one: f32 = -1.0;
    let zero: f32 = 0.0;
    let neg_zero: f32 = -0.0;
    let one: f32 = 1.0;
    let other: f32 = 42.0;

    classify_f32(neg_one);
    classify_f32(zero);
    classify_f32(neg_zero); // -0.0 must match 0.0 arm (== semantics)
    classify_f32(one);
    classify_f32(other);

    0
}
