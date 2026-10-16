// Test that the types of distinct fn items are not compatible by
// default. See also `run-pass/fn-item-type-*.rs`.

fn foo<T>(x: isize) -> isize { x * 2 }
fn bar<T>(x: isize) -> isize { x * 4 }

fn eq<T>(x: T, y: T) { }

trait Foo { fn foo() { /* this is a default fn */ } }
impl<T> Foo for T { /* `foo` is still default here */ }

fn main() {
    eq(foo::<u8>, bar::<u8>);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
// { dg-error ".E0308." "" { target *-*-* } .-5 }
// { dg-error ".E0308." "" { target *-*-* } .-6 }
// { dg-error ".E0308." "" { target *-*-* } .-7 }

    eq(foo::<u8>, foo::<i8>);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
// { dg-error ".E0308." "" { target *-*-* } .-5 }

    eq(bar::<String>, bar::<Vec<u8>>);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
// { dg-error ".E0308." "" { target *-*-* } .-5 }
// { dg-error ".E0308." "" { target *-*-* } .-6 }
// { dg-error ".E0308." "" { target *-*-* } .-7 }

    // Make sure we distinguish between trait methods correctly.
    eq(<u8 as Foo>::foo, <u16 as Foo>::foo);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
// { dg-error ".E0308." "" { target *-*-* } .-5 }

    eq(foo::<u8>, bar::<u8> as fn(isize) -> isize);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
// { dg-error ".E0308." "" { target *-*-* } .-5 }
// { dg-error ".E0308." "" { target *-*-* } .-6 }

    eq(foo::<u8> as fn(isize) -> isize, bar::<u8>); // ok!
}

