trait MyTrait {}
impl !MyTrait for u32 {} // { dg-error ".E0658." "" { target *-*-* } }
fn main() {}

