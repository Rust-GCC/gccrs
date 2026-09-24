#![feature(negative_impls)]
#![feature(marker_trait_attr)]

#[marker]
trait MyTrait {}

struct TestType<T>(::std::marker::PhantomData<T>);

unsafe impl<T: MyTrait + 'static> Send for TestType<T> {}

impl<T: MyTrait> !Send for TestType<T> {} // { dg-error ".E0751." "" { target *-*-* } }

unsafe impl<T: 'static> Send for TestType<T> {} // { dg-error ".E0119." "" { target *-*-* } }

impl !Send for TestType<i32> {}

fn main() {}

