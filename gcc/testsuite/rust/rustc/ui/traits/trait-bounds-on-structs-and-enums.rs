trait Trait {}

struct Foo<T:Trait> {
    x: T,
}

enum Bar<T:Trait> {
    ABar(isize),
    BBar(T),
    CBar(usize),
}

impl<T> Foo<T> {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    fn uhoh() {}
}

struct Baz {
    a: Foo<isize>, // { dg-error ".E0277." "" { target *-*-* } }
}

enum Boo {
    Quux(Bar<usize>), // { dg-error ".E0277." "" { target *-*-* } }
}

struct Badness<U> {
    b: Foo<U>, // { dg-error ".E0277." "" { target *-*-* } }
}

enum MoreBadness<V> {
    EvenMoreBadness(Bar<V>), // { dg-error ".E0277." "" { target *-*-* } }
}

struct TupleLike(
    Foo<i32>, // { dg-error ".E0277." "" { target *-*-* } }
);

enum Enum {
    DictionaryLike { field: Bar<u8> }, // { dg-error ".E0277." "" { target *-*-* } }
}

fn main() {
}

