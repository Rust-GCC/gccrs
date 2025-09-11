#![allow(dead_code)]

// Test that we report an error for unused type parameters in types and traits,
// and that we offer a helpful suggestion.

struct SomeStruct<A> { x: u32 }
// { dg-error ".E0392." "" { target *-*-* } .-1 }

enum SomeEnum<A> { Nothing }
// { dg-error ".E0392." "" { target *-*-* } .-1 }

// Here T might *appear* used, but in fact it isn't.
enum ListCell<T> {
// { dg-error ".E0392." "" { target *-*-* } .-1 }
    Cons(Box<ListCell<T>>),
    Nil
}

fn main() {}

