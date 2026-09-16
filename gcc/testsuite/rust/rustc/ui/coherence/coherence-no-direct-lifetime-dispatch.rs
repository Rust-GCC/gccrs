// Test that you cannot *directly* dispatch on lifetime requirements

trait MyTrait { fn foo() {} }

impl<T> MyTrait for T {}
impl<T: 'static> MyTrait for T {}
// { dg-error ".E0119." "" { target *-*-* } .-1 }

fn main() {}

