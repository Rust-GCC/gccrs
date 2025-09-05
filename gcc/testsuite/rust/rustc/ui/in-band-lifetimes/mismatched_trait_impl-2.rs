use std::ops::Deref;
trait Trait {}

struct Struct;

impl Deref for Struct {
    type Target = dyn Trait;
    fn deref(&self) -> &dyn Trait {
// { dg-error "" "" { target *-*-* } .-1 }
        unimplemented!();
    }
}

fn main() {}

