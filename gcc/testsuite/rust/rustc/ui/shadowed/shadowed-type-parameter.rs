// Test that shadowed lifetimes generate an error.

#![feature(box_syntax)]

struct Foo<T>(T);

impl<T> Foo<T> {
    fn shadow_in_method<T>(&self) {}
// { dg-error ".E0403." "" { target *-*-* } .-1 }

    fn not_shadow_in_item<U>(&self) {
        struct Bar<T, U>(T,U); // not a shadow, separate item
        fn foo<T, U>() {} // same
    }
}

trait Bar<T> {
    fn dummy(&self) -> T;

    fn shadow_in_required<T>(&self);
// { dg-error ".E0403." "" { target *-*-* } .-1 }

    fn shadow_in_provided<T>(&self) {}
// { dg-error ".E0403." "" { target *-*-* } .-1 }

    fn not_shadow_in_required<U>(&self);
    fn not_shadow_in_provided<U>(&self) {}
}

fn main() {}

