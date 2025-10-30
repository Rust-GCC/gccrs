// When explicit discriminant value has
// a type that does not match the representation
// type, rustc should fail gracefully.

// See also run-pass/discrim-explicit-23030.rs where the input types
// are correct.

#![allow(dead_code, unused_variables, unused_imports)]

use std::{i8,u8,i16,u16,i32,u32,i64, u64};

fn f_i8() {
    #[repr(i8)]
    enum A {
        Ok = i8::MAX - 1,
        Ok2,
        OhNo = 0_u8,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    }

    let x = A::Ok;
}

fn f_u8() {
    #[repr(u8)]
    enum A {
        Ok = u8::MAX - 1,
        Ok2,
        OhNo = 0_i8,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    }

    let x = A::Ok;
}

fn f_i16() {
    #[repr(i16)]
    enum A {
        Ok = i16::MAX - 1,
        Ok2,
        OhNo = 0_u16,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    }

    let x = A::Ok;
}

fn f_u16() {
    #[repr(u16)]
    enum A {
        Ok = u16::MAX - 1,
        Ok2,
        OhNo = 0_i16,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    }

    let x = A::Ok;
}

fn f_i32() {
    #[repr(i32)]
    enum A {
        Ok = i32::MAX - 1,
        Ok2,
        OhNo = 0_u32,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    }

    let x = A::Ok;
}

fn f_u32() {
    #[repr(u32)]
    enum A {
        Ok = u32::MAX - 1,
        Ok2,
        OhNo = 0_i32,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    }

    let x = A::Ok;
}

fn f_i64() {
    #[repr(i64)]
    enum A {
        Ok = i64::MAX - 1,
        Ok2,
        OhNo = 0_u64,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    }

    let x = A::Ok;
}

fn f_u64() {
    #[repr(u64)]
    enum A {
        Ok = u64::MAX - 1,
        Ok2,
        OhNo = 0_i64,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    }

    let x = A::Ok;
}

fn main() { }

