#![feature(specialization)] // { dg-warning "" "" { target *-*-* } }
#![feature(negative_impls)]

trait MyTrait {}

impl<T> !MyTrait for T {}
impl MyTrait for u32 {} // { dg-error ".E0751." "" { target *-*-* } }

fn main() {}

