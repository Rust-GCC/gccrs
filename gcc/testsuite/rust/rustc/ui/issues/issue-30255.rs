//
// Test that lifetime elision error messages correctly omit parameters
// with no elided lifetimes

struct S<'a> {
    field: &'a i32,
}

fn f(a: &S, b: i32) -> &i32 {
// { dg-error ".E0106." "" { target *-*-* } .-1 }
    panic!();
}

fn g(a: &S, b: bool, c: &i32) -> &i32 {
// { dg-error ".E0106." "" { target *-*-* } .-1 }
    panic!();
}

fn h(a: &bool, b: bool, c: &S, d: &i32) -> &i32 {
// { dg-error ".E0106." "" { target *-*-* } .-1 }
    panic!();
}

fn main() {}

