#![feature(rustc_attrs, const_fn)]

#[rustc_args_required_const(0)]
fn foo(_a: i32) {
}

fn main() {
    let a = foo; // { dg-error "" "" { target *-*-* } }
    a(2);
}

