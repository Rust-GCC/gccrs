use std::any::Any;
use std::any::TypeId;

trait Private<P, R> {
    fn call(&self, p: P, r: R);
}
pub trait Public: Private<
// { dg-error ".E0445." "" { target *-*-* } .-1 }
    <Self as Public>::P,
    <Self as Public>::R
> {
    type P;
    type R;

    fn call_inner(&self);
}

fn main() {}

