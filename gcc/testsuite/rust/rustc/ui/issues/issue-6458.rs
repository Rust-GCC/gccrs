use std::marker;

pub struct TypeWithState<State>(marker::PhantomData<State>);
pub struct MyState;

pub fn foo<State>(_: TypeWithState<State>) {}

pub fn bar() {
   foo(TypeWithState(marker::PhantomData));
// { dg-error ".E0282." "" { target *-*-* } .-1 }
}

fn main() {
}

