#![feature(negative_impls)]
#![feature(specialization)] // { dg-warning "" "" { target *-*-* } }

trait MyTrait {}

struct TestType<T>(::std::marker::PhantomData<T>);

unsafe impl<T: Clone> Send for TestType<T> {}
impl<T: MyTrait> !Send for TestType<T> {} // { dg-error ".E0751." "" { target *-*-* } }

fn main() {}

