#![allow(unused)]

// ignore-tidy-linelength

#[derive(Clone, Copy, Default)]
struct S {
    a: u8,
    b: u8,
}
#[derive(Clone, Copy, Default)]
struct Z {
    c: u8,
    d: u8,
}

union U {
    s: S,
    z: Z,
}

fn main() {
    unsafe {
        let mut u = U { s: Default::default() };

        let mref = &mut u.s.a;
        *mref = 22;

        let nref = &u.z.c;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
        println!("{} {}", mref, nref)
    }
}

