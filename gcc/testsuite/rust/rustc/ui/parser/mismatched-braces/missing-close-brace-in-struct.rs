pub(crate) struct Bar<T> {
  foo: T,

trait T { // { dg-error "" "" { target *-*-* } }
    fn foo(&self);
}


impl T for Bar<usize> {
fn foo(&self) {}
}

fn main() {} // { dg-error "" "" { target *-*-* } }

