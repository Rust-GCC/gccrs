#[cfg] // { dg-error "" "" { target *-*-* } }
struct S1;

#[cfg = 10] // { dg-error "" "" { target *-*-* } }
struct S2;

#[cfg()] // { dg-error "" "" { target *-*-* } }
struct S3;

#[cfg(a, b)] // { dg-error "" "" { target *-*-* } }
struct S4;

#[cfg("str")] // { dg-error "" "" { target *-*-* } }
struct S5;

#[cfg(a::b)] // { dg-error "" "" { target *-*-* } }
struct S6;

#[cfg(a())] // { dg-error ".E0537." "" { target *-*-* } }
struct S7;

#[cfg(a = 10)] // { dg-error ".E0565." "" { target *-*-* } }
struct S8;

#[cfg(a = b"hi")]  // { dg-error ".E0565." "" { target *-*-* } }
struct S9;

macro_rules! generate_s10 {
    ($expr: expr) => {
        #[cfg(feature = $expr)]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        struct S10;
    }
}

generate_s10!(concat!("nonexistent"));

fn main() {}

