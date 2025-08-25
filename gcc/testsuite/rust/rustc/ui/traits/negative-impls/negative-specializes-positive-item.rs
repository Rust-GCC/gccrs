#![feature(specialization)] // { dg-warning "" "" { target *-*-* } }
#![feature(negative_impls)]

// Negative impl for u32 cannot "specialize" the base impl.
trait MyTrait {
    fn foo();
}
impl<T> MyTrait for T {
    default fn foo() {}
}
impl !MyTrait for u32 {} // { dg-error ".E0751." "" { target *-*-* } }

fn main() {}

