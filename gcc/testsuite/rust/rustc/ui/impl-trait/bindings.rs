#![feature(impl_trait_in_bindings)]
// { dg-warning "" "" { target *-*-* } .-1 }

fn a<T: Clone>(x: T) {
    const foo: impl Clone = x;
// { dg-error ".E0435." "" { target *-*-* } .-1 }
}

fn b<T: Clone>(x: T) {
    let _ = move || {
        const foo: impl Clone = x;
// { dg-error ".E0435." "" { target *-*-* } .-1 }
    };
}

trait Foo<T: Clone> {
    fn a(x: T) {
        const foo: impl Clone = x;
// { dg-error ".E0435." "" { target *-*-* } .-1 }
    }
}

impl<T: Clone> Foo<T> for i32 {
    fn a(x: T) {
        const foo: impl Clone = x;
// { dg-error ".E0435." "" { target *-*-* } .-1 }
    }
}

fn main() { }

