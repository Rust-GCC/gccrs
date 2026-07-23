#![feature(fn_traits)]

trait CallSingle<A, B> {
    fn call(&self, a: A) -> B where Self: Sized, Self: Fn(A) -> B;
}

impl<A, B, F: Fn(A) -> B> CallSingle<A, B> for F {
    fn call(&self, a: A) -> B {
        <Self as Fn(A) -> B>::call(self, (a,))
// { dg-error ".E0229." "" { target *-*-* } .-1 }
    }
}

fn main() {}

