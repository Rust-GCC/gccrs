#![feature(rustc_attrs)]

#[rustc_args_required_const(0)] // { dg-error "" "" { target *-*-* } }
fn foo1() {}

#[rustc_args_required_const(1)] // { dg-error "" "" { target *-*-* } }
fn foo2(_: u8) {}

#[rustc_args_required_const(a)] // { dg-error "" "" { target *-*-* } }
fn foo4() {}

#[rustc_args_required_const(1, a, 2, b)] // { dg-error "" "" { target *-*-* } }
fn foo5(_: u8, _: u8, _: u8) {}

#[rustc_args_required_const(0)] // { dg-error "" "" { target *-*-* } }
struct S;

#[rustc_args_required_const(0usize)] // { dg-error "" "" { target *-*-* } }
fn foo6(_: u8) {}

extern {
    #[rustc_args_required_const(1)] // { dg-error "" "" { target *-*-* } }
    fn foo7(_: u8);
}

fn main() {}

