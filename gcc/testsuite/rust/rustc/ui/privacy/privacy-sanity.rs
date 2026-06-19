#![feature(negative_impls)]

pub trait Tr {
    fn f();
    const C: u8;
    type T;
}
pub struct S {
    pub a: u8
}
struct Ts(pub u8);

pub impl Tr for S {  // { dg-error ".E0449." "" { target *-*-* } }
    pub fn f() {} // { dg-error ".E0449." "" { target *-*-* } }
    pub const C: u8 = 0; // { dg-error ".E0449." "" { target *-*-* } }
    pub type T = u8; // { dg-error ".E0449." "" { target *-*-* } }
}
pub impl S { // { dg-error ".E0449." "" { target *-*-* } }
    pub fn f() {}
    pub const C: u8 = 0;
    // pub type T = u8;
}
pub extern "C" { // { dg-error ".E0449." "" { target *-*-* } }
    pub fn f();
    pub static St: u8;
}

const MAIN: u8 = {
    pub trait Tr {
        fn f();
        const C: u8;
        type T;
    }
    pub struct S {
        pub a: u8
    }
    struct Ts(pub u8);

    pub impl Tr for S {  // { dg-error ".E0449." "" { target *-*-* } }
        pub fn f() {} // { dg-error ".E0449." "" { target *-*-* } }
        pub const C: u8 = 0; // { dg-error ".E0449." "" { target *-*-* } }
        pub type T = u8; // { dg-error ".E0449." "" { target *-*-* } }
    }
    pub impl S { // { dg-error ".E0449." "" { target *-*-* } }
        pub fn f() {}
        pub const C: u8 = 0;
        // pub type T = u8;
    }
    pub extern "C" { // { dg-error ".E0449." "" { target *-*-* } }
        pub fn f();
        pub static St: u8;
    }

    0
};

fn main() {
    pub trait Tr {
        fn f();
        const C: u8;
        type T;
    }
    pub struct S {
        pub a: u8
    }
    struct Ts(pub u8);

    pub impl Tr for S {  // { dg-error ".E0449." "" { target *-*-* } }
        pub fn f() {} // { dg-error ".E0449." "" { target *-*-* } }
        pub const C: u8 = 0; // { dg-error ".E0449." "" { target *-*-* } }
        pub type T = u8; // { dg-error ".E0449." "" { target *-*-* } }
    }
    pub impl S { // { dg-error ".E0449." "" { target *-*-* } }
        pub fn f() {}
        pub const C: u8 = 0;
        // pub type T = u8;
    }
    pub extern "C" { // { dg-error ".E0449." "" { target *-*-* } }
        pub fn f();
        pub static St: u8;
    }
}

