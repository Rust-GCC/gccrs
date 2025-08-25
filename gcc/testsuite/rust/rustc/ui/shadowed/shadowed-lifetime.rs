// Test that shadowed lifetimes generate an error.

struct Foo<'a>(&'a isize);

impl<'a> Foo<'a> {
    fn shadow_in_method<'a>(&'a self) -> &'a isize {
// { dg-error ".E0496." "" { target *-*-* } .-1 }
        self.0
    }

    fn shadow_in_type<'b>(&'b self) -> &'b isize {
        let x: for<'b> fn(&'b isize) = panic!();
// { dg-error ".E0496." "" { target *-*-* } .-1 }
        self.0
    }

    fn not_shadow_in_item<'b>(&'b self) {
        struct Bar<'a, 'b>(&'a isize, &'b isize); // not a shadow, separate item
        fn foo<'a, 'b>(x: &'a isize, y: &'b isize) { } // same
    }
}

fn main() {
}

