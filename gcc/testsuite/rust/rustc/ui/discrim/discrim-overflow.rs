// Issue 23030: Detect overflowing discriminant

// See also run-pass/discrim-explicit-23030.rs where the suggested
// workaround is tested.

use std::{i8,u8,i16,u16,i32,u32,i64, u64};

fn f_i8() {
    #[repr(i8)]
    enum A {
        Ok = i8::MAX - 1,
        Ok2,
        OhNo, // { dg-error ".E0370." "" { target *-*-* } }
    }

    let x = A::Ok;
}

fn f_u8() {
    #[repr(u8)]
    enum A {
        Ok = u8::MAX - 1,
        Ok2,
        OhNo, // { dg-error ".E0370." "" { target *-*-* } }
    }

    let x = A::Ok;
}

fn f_i16() {
    #[repr(i16)]
    enum A {
        Ok = i16::MAX - 1,
        Ok2,
        OhNo, // { dg-error ".E0370." "" { target *-*-* } }
    }

    let x = A::Ok;
}

fn f_u16() {
    #[repr(u16)]
    enum A {
        Ok = u16::MAX - 1,
        Ok2,
        OhNo, // { dg-error ".E0370." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    }

    let x = A::Ok;
}

fn f_i32() {
    #[repr(i32)]
    enum A {
        Ok = i32::MAX - 1,
        Ok2,
        OhNo, // { dg-error ".E0370." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    }

    let x = A::Ok;
}

fn f_u32() {
    #[repr(u32)]
    enum A {
        Ok = u32::MAX - 1,
        Ok2,
        OhNo, // { dg-error ".E0370." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    }

    let x = A::Ok;
}

fn f_i64() {
    #[repr(i64)]
    enum A {
        Ok = i64::MAX - 1,
        Ok2,
        OhNo, // { dg-error ".E0370." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    }

    let x = A::Ok;
}

fn f_u64() {
    #[repr(u64)]
    enum A {
        Ok = u64::MAX - 1,
        Ok2,
        OhNo, // { dg-error ".E0370." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    }

    let x = A::Ok;
}

fn main() { }

