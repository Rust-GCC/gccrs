trait T {
    fn foo(&self);

pub(crate) struct Bar<T>();
// { dg-error "" "" { target *-*-* } .-1 }

impl T for Bar<usize> {
// { dg-error "" "" { target *-*-* } .-1 }
fn foo(&self) {}
}

fn main() {} // { dg-error "" "" { target *-*-* } }

