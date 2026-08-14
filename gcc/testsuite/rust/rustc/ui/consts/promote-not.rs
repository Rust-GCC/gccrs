// ignore-tidy-linelength
// Test various things that we do not want to promote.
#![allow(unconditional_panic, const_err)]
#![feature(const_fn, const_fn_union)]

// We do not promote mutable references.
static mut TEST1: Option<&mut [i32]> = Some(&mut [1, 2, 3]); // { dg-error ".E0716." "" { target *-*-* } }

static mut TEST2: &'static mut [i32] = {
    let x = &mut [1,2,3]; // { dg-error ".E0716." "" { target *-*-* } }
    x
};

// We do not promote fn calls in `fn`, including `const fn`.
pub const fn promote_cal(b: bool) -> i32 {
    const fn foo() { [()][42] }

    if b {
        let _x: &'static () = &foo(); // { dg-error ".E0716." "" { target *-*-* } }
    }
    13
}

// We do not promote union field accesses in `fn.
union U { x: i32, y: i32 }
pub const fn promote_union() {
    let _x: &'static i32 = &unsafe { U { x: 0 }.x }; // { dg-error ".E0716." "" { target *-*-* } }
}

// We do not promote union field accesses in `const`, either.
const TEST_UNION: () = {
    let _x: &'static i32 = &unsafe { U { x: 0 }.x }; // { dg-error ".E0716." "" { target *-*-* } }
};

fn main() {}

