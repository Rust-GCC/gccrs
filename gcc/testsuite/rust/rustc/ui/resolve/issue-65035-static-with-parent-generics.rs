#![feature(const_generics)]
// { dg-warning "" "" { target *-*-* } .-1 }

fn f<T>() {
    extern "C" {
        static a: *const T;
// { dg-error ".E0401." "" { target *-*-* } .-1 }
    }
}

fn g<T: Default>() {
    static a: *const T = Default::default();
// { dg-error ".E0401." "" { target *-*-* } .-1 }
}

fn h<const N: usize>() {
    extern "C" {
        static a: [u8; N];
// { dg-error ".E0401." "" { target *-*-* } .-1 }
    }
}

fn i<const N: usize>() {
    static a: [u8; N] = [0; N];
// { dg-error ".E0401." "" { target *-*-* } .-1 }
// { dg-error ".E0401." "" { target *-*-* } .-2 }
}

fn main() {}

