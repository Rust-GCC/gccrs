// build-fail
// compile-flags:-Zpolymorphize=on
#![feature(fn_traits, rustc_attrs, unboxed_closures)]

// This test checks that the polymorphization analysis considers a closure
// as using all generic parameters if it does an unsizing cast.

#[rustc_polymorphize_error]
fn foo<T: Default>() {
    let _: T = Default::default();
    (|| Box::new(|| {}) as Box<dyn Fn()>)();
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

#[rustc_polymorphize_error]
fn foo2<T: Default>() {
    let _: T = Default::default();
    (|| {
// { dg-error "" "" { target *-*-* } .-1 }
        let call: extern "rust-call" fn(_, _) = Fn::call;
        call(&|| {}, ());
// { dg-error "" "" { target *-*-* } .-1 }
    })();
}

fn main() {
    foo::<u32>();
    foo2::<u32>();
}

