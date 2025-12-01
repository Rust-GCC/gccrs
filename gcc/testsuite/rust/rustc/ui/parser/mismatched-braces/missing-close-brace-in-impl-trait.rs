fn main() {}

impl T for () { // { dg-error ".E0405." "" { target *-*-* } }

fn foo(&self) {}

trait T { // { dg-error "" "" { target *-*-* } }
    fn foo(&self);
}

pub(crate) struct Bar<T>(); // { dg-error "" "" { target *-*-* } }

// { dg-error "" "" { target *-*-* } }

