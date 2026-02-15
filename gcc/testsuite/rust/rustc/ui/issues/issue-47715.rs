trait Foo {}

trait Bar<T> {}

trait Iterable {
    type Item;
}

struct Container<T: Iterable<Item = impl Foo>> {
// { dg-error ".E0562." "" { target *-*-* } .-1 }
    field: T
}

enum Enum<T: Iterable<Item = impl Foo>> {
// { dg-error ".E0562." "" { target *-*-* } .-1 }
    A(T),
}

union Union<T: Iterable<Item = impl Foo> + Copy> {
// { dg-error ".E0562." "" { target *-*-* } .-1 }
    x: T,
}

type Type<T: Iterable<Item = impl Foo>> = T;
// { dg-error ".E0562." "" { target *-*-* } .-1 }

fn main() {
}

