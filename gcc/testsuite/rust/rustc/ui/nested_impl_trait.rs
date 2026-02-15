use std::fmt::Debug;

fn fine(x: impl Into<u32>) -> impl Into<u32> { x }

fn bad_in_ret_position(x: impl Into<u32>) -> impl Into<impl Debug> { x }
// { dg-error ".E0666." "" { target *-*-* } .-1 }

fn bad_in_fn_syntax(x: fn() -> impl Into<impl Debug>) {}
// { dg-error ".E0562." "" { target *-*-* } .-1 }
// { dg-error ".E0562." "" { target *-*-* } .-2 }

fn bad_in_arg_position(_: impl Into<impl Debug>) { }
// { dg-error ".E0666." "" { target *-*-* } .-1 }

struct X;
impl X {
    fn bad(x: impl Into<u32>) -> impl Into<impl Debug> { x }
// { dg-error ".E0666." "" { target *-*-* } .-1 }
}

fn allowed_in_assoc_type() -> impl Iterator<Item=impl Fn()> {
    vec![|| println!("woot")].into_iter()
}

fn allowed_in_ret_type() -> impl Fn() -> impl Into<u32> {
// { dg-error ".E0562." "" { target *-*-* } .-1 }
    || 5
}

fn main() {}

