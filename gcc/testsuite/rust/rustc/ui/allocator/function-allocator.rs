#[global_allocator]
fn foo() {} // { dg-error "" "" { target *-*-* } }

fn main() {}

