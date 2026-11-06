// { dg-output "neg one\r*\nzero\r*\nzero\r*\none\r*\nother\r*\n" }
// Tests basic floating-point literal pattern matching, including
// that -0.0 and 0.0 are treated as equal (== semantics, not bitwise).
#![feature(no_core)]
#![no_core]

extern "C" {
    fn printf(s: *const i8, ...);
}

fn classify(x: f64) {
    match x {
        -1.0 => unsafe {
            let a = "neg one\n\0";
            let b = a as *const str;
            let c = b as *const i8;
            printf(c);
        },
        0.0 => unsafe {
            let a = "zero\n\0";
            let b = a as *const str;
            let c = b as *const i8;
            printf(c);
        },
        1.0 => unsafe {
            let a = "one\n\0";
            let b = a as *const str;
            let c = b as *const i8;
            printf(c);
        },
        _ => unsafe {
            let a = "other\n\0";
            let b = a as *const str;
            let c = b as *const i8;
            printf(c);
        },
    }
}

fn main() -> i32 {
    classify(-1.0);
    classify(0.0);
    classify(-0.0); // -0.0 must match 0.0 arm (== semantics)
    classify(1.0);
    classify(999.0);

    0
}
